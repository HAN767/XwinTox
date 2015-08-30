#include <cstring>
#include <libgen.h>

#include "MCOMMTOX.h"
#include "xwintox.h"

void MCOMMTOX::recvSignal (unsigned int dwType, PBMessage_t * msg)
{
    switch (dwType)
    {
    case clSaveData:
        saveToxData_ ();
        break;

    case frSendMsg:
        dbg ("Send message to %d\n", msg->I1);
        mtx_lock (&mtxTox_);
        tox_friend_send_message (tox_, msg->I1, TOX_MESSAGE_TYPE_NORMAL,
                                 (uint8_t *)msg->S1, strlen (msg->S1), 0);
        mtx_unlock (&mtxTox_);
        break;

    case frDelete:
        dbg ("Delete friend %d\n", msg->I1);
        mtx_lock (&mtxTox_);
        tox_friend_delete (tox_, msg->I1, 0);
        mtx_unlock (&mtxTox_);
        saveToxData_ ();
        break;

    case frAcceptRequest:
        acceptFriendRequest_ (msg->S1);
        saveToxData_ ();
        break;

    case frSendRequest:
        dbg ("Add friend\n");
        addFriend_ (msg->S1, msg->S2);
        break;

    case ftResume:
        dbg ("Resume transfer\n");
        controlFileTransfer (msg->I1, msg->I2, TC_Resume);
        break;

    case ftSend:
        PBMessage_t * msgFtSendRequest;
        TOX_ERR_FILE_SEND errSend;
        unsigned int dwFilenum;
        char * szFilename;

        szFilename = strdup (basename (msg->S1));

        mtx_lock (&mtxTox_);
        dwFilenum =
            tox_file_send (tox_, msg->I1, TOX_FILE_KIND_DATA, msg->I2, NULL,
                           const_cast< const uint8_t * > (
                               reinterpret_cast< uint8_t * > (szFilename)),
                           strlen (szFilename), &errSend);
        mtx_unlock (&mtxTox_);

        free (szFilename);

        dbg ("Transfer %s to %d. Error: %d. %s %s\n", szFilename, msg->I1,
             errSend, msg->S1, szFilename);

        if (errSend != TOX_ERR_FILE_SEND_OK)
            return;

        msgFtSendRequest = PB_New_Message ();
        msgFtSendRequest->I1 = msg->I1;
        msgFtSendRequest->I2 = dwFilenum;
        msgFtSendRequest->I3 = msg->I2;
        msgFtSendRequest->S1 = strdup (msg->S1);
        msgFtSendRequest->S2 = strdup (msg->S1);

        xwfDispatch (ftSendRequest, msgFtSendRequest);

        break;

    case ftBytesDelivery:
        tox_file_send_chunk (tox_, msg->I1, msg->I2, msg->I3, (uint8_t *)msg->V,
                             msg->I4, 0);
        break;
    }
}

void MCOMMTOX::controlFileTransfer (unsigned int dwFID, unsigned int dwTID,
                                    unsigned int eCtrl)
{
    TOX_ERR_FILE_CONTROL err;
    TOX_FILE_CONTROL c = getToxFC (eCtrl);

    mtx_lock (&mtxTox_);

    if (tox_file_control (tox_, dwFID, dwTID, c, &err))
    {
        PBMessage_t * msg = PB_New_Message ();

        dbg ("success\n");

        msg->I1 = dwFID;
        msg->I2 = dwTID;

        if (c == TOX_FILE_CONTROL_RESUME)
            msg->I3 = TC_Resume;
        else if (c == TOX_FILE_CONTROL_PAUSE)
            msg->I3 = TC_Pause;
        else if (c == TOX_FILE_CONTROL_CANCEL)
            msg->I3 = TC_Cancel;

        xwfDispatch (ftControl, msg);
    }

    mtx_unlock (&mtxTox_);
}

TOX_FILE_CONTROL MCOMMTOX::getToxFC (unsigned int eCtrl)
{
    if (eCtrl == TC_Resume)
        return TOX_FILE_CONTROL_RESUME;
    else if (eCtrl == TC_Pause)
        return TOX_FILE_CONTROL_PAUSE;
    else if (eCtrl == TC_Cancel)
        return TOX_FILE_CONTROL_CANCEL;
    else
        return static_cast< TOX_FILE_CONTROL > (0);
}

unsigned int MCOMMTOX::getXwFC (TOX_FILE_CONTROL tCtrl)
{
    if (tCtrl == TOX_FILE_CONTROL_RESUME)
        return TC_Resume;
    else if (tCtrl == TOX_FILE_CONTROL_PAUSE)
        return TC_Pause;
    else if (tCtrl == TOX_FILE_CONTROL_CANCEL)
        return TC_Cancel;
    else
        return static_cast< TOX_FILE_CONTROL > (0);
}
