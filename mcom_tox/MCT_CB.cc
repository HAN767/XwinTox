#include <cstring>

#include "hexstring.h"
#include "xwintox.h"
#include "MCOMMTOX.h"

#define TOXCALLBACK(func)                                                      \
    tox_callback_##func (                                                      \
        tox_,                                                                  \
        &thunk< decltype (&MCOMMTOX::cb_##func), &MCOMMTOX::cb_##func >::call, \
        this);

void MCOMMTOX::registerCallbacks_ ()
{
    TOXCALLBACK (self_connection_status)

    TOXCALLBACK (friend_connection_status)
    TOXCALLBACK (friend_status)
    TOXCALLBACK (friend_name)
    TOXCALLBACK (friend_message)
    TOXCALLBACK (friend_request)

    TOXCALLBACK (file_recv)
    TOXCALLBACK (file_recv_control)
    TOXCALLBACK (file_recv_chunk)

    TOXCALLBACK (file_chunk_request)
}

void MCOMMTOX::cb_self_connection_status (TOX_CONNECTION connection_status)
{
    unsigned int wCStatus;
    PBMessage_t * msgCStatus = PB_New_Message ();

    if (connection_status == TOX_CONNECTION_NONE)
        wCStatus = 0;
    else if (connection_status == TOX_CONNECTION_TCP)
        wCStatus = 1;
    else if (connection_status == TOX_CONNECTION_UDP)
        wCStatus = 1;
    else
        wCStatus = 0;

    dbg ("Connection status changed: %d\n", wCStatus);
    msgCStatus->I1 = wCStatus;
    xwfDispatch (clConn, msgCStatus);
}

void MCOMMTOX::cb_friend_connection_status (uint32_t friend_number,
                                            TOX_CONNECTION connection_status)
{
    unsigned int wCStatus;
    TOX_USER_STATUS tuStatus;
    PBMessage_t * msgCStatus = PB_New_Message ();

    if (connection_status == TOX_CONNECTION_NONE)
        wCStatus = stOffline;
    else if (connection_status == TOX_CONNECTION_TCP)
        wCStatus = stOnline;
    else if (connection_status == TOX_CONNECTION_UDP)
        wCStatus = stOnline;
    else
        wCStatus = stOffline;

    if (wCStatus == stOnline)
    {
        tuStatus = tox_friend_get_status (tox_, friend_number, 0);

        switch (tuStatus)
        {
        case TOX_USER_STATUS_NONE:
            wCStatus = stOnline;
            break;

        case TOX_USER_STATUS_AWAY:
            wCStatus = stAway;
            break;

        case TOX_USER_STATUS_BUSY:
            wCStatus = stBusy;
            break;
        }
    }

    msgCStatus->I1 = friend_number;
    msgCStatus->I2 = wCStatus;
    xwfDispatch (frStatus, msgCStatus);
}

void MCOMMTOX::cb_friend_status (uint32_t friend_number, TOX_USER_STATUS status)
{
    unsigned int wCStatus;
    PBMessage_t * msgCStatus = PB_New_Message ();

    switch (status)
    {
    case TOX_USER_STATUS_NONE:
        wCStatus = stOnline;
        break;

    case TOX_USER_STATUS_AWAY:
        wCStatus = stAway;
        break;

    case TOX_USER_STATUS_BUSY:
        wCStatus = stBusy;
        break;
    }

    msgCStatus->I1 = friend_number;
    msgCStatus->I2 = wCStatus;
    xwfDispatch (frStatus, msgCStatus);
}

void MCOMMTOX::cb_friend_name (uint32_t friend_number, const uint8_t * name,
                               size_t length)
{
    PBMessage_t * msgFrName = PB_New_Message ();

    msgFrName->I1 = friend_number;
    msgFrName->S1 = strndup ((const char *)name, length);
    xwfDispatch (frName, msgFrName);
}

void MCOMMTOX::cb_friend_message (uint32_t friend_number, TOX_MESSAGE_TYPE type,
                                  const uint8_t * message, size_t length)
{
    PBMessage_t * msgFrMsg = PB_New_Message ();

    msgFrMsg->I1 = friend_number;
    msgFrMsg->S1 = strndup ((const char *)message, length);
    xwfDispatch (frMsg, msgFrMsg);
}

void MCOMMTOX::cb_friend_request (const uint8_t * public_key,
                                  const uint8_t * message, size_t length)
{
    PBMessage_t * msgFrRequest = PB_New_Message ();

    msgFrRequest->S1 = bin_to_hex_string (public_key, TOX_PUBLIC_KEY_SIZE);
    msgFrRequest->S2 = strndup ((const char *)message, length);

    xwfDispatch (frRequest, msgFrRequest);
}

void MCOMMTOX::cb_file_recv (uint32_t friend_number, uint32_t file_number,
                             uint32_t kind, uint64_t file_size,
                             const uint8_t * filename, size_t filename_length)
{
    if (kind == TOX_FILE_KIND_AVATAR)
    {
        char szFNAvEnd[31];
        std::string strFNAv;
        AvatarTransfer avtransNew;

        snprintf (szFNAvEnd, 31, "%d.png", friend_number);
        strFNAv.assign (pszxwfCall ("APP/GetMiscDataFilename", szFNAvEnd));

        avtransNew.file = fopen (strFNAv.c_str (), "wb");
        avtransNew.uFrNum = friend_number;
        avtransNew.uTNum = file_number;
        avtransNew.szFilename = strdup (strFNAv.c_str ());

        dbg ("Avatar from %d: %s\n", friend_number, strFNAv.c_str ());

        vecAvTransfers.push_back (avtransNew);

        tox_file_control (tox_, friend_number, file_number,
                          TOX_FILE_CONTROL_RESUME, NULL);
        return;
    }
    else if (kind != TOX_FILE_KIND_DATA)
    {
        tox_file_control (tox_, friend_number, file_number,
                          TOX_FILE_CONTROL_CANCEL, NULL);
        return;
    }

    PBMessage_t * msgFtRequest = PB_New_Message ();

    msgFtRequest->S1 =
        strndup (reinterpret_cast< const char * > (filename), filename_length);
    msgFtRequest->I1 = friend_number;
    msgFtRequest->I2 = file_number;
    msgFtRequest->I3 = file_size;

    xwfDispatch (ftRequest, msgFtRequest);
}

void MCOMMTOX::cb_file_recv_chunk (uint32_t friend_number, uint32_t file_number,
                                   uint64_t position, const uint8_t * data,
                                   size_t length)
{
    bool erase = false;
    std::vector< AvatarTransfer >::iterator avTrC = vecAvTransfers.begin ();
    for (; avTrC != vecAvTransfers.end (); avTrC++)
    {
        if ((avTrC->uTNum == file_number) && (avTrC->uFrNum == friend_number))
        {
            if (length > 0)
            {
                fseek (avTrC->file, position, SEEK_SET);
                fwrite (data, length, 1, avTrC->file);
                return;
            }
            else
            {
                PBMessage_t * msgAvDownloaded = PB_New_Message ();
                dbg ("Avatar transfer complete\n");
                fclose (avTrC->file);
                erase = true;

                msgAvDownloaded->S1 = strdup (avTrC->szFilename);
                msgAvDownloaded->I1 = friend_number;
                xwfDispatch (frAvDownloaded, msgAvDownloaded);
                break;
            }
        }
    }

    if (erase)
    {
        free (avTrC->szFilename);
        vecAvTransfers.erase (avTrC);
        return;
    }

    PBMessage_t * msgFtBytes = PB_New_Message ();
    // dbg("Receive chunk\n");

    msgFtBytes->I1 = friend_number;
    msgFtBytes->I2 = file_number;
    msgFtBytes->I3 = position;
    msgFtBytes->I4 = length;
    msgFtBytes->V = malloc (sizeof (char) * length);
    memcpy (msgFtBytes->V,
            const_cast< void * > (static_cast< const void * > (data)), length);

    xwfDispatch (ftBytes, msgFtBytes);
}
void MCOMMTOX::cb_file_recv_control (uint32_t friend_number,
                                     uint32_t file_number, TOX_FILE_CONTROL c)
{
    printf ("FILE RECV CONTROL: %d\n", c);

    PBMessage_t * msg = PB_New_Message ();

    msg->I1 = friend_number;
    msg->I2 = file_number;
    msg->I3 = getXwFC (c);
    xwfDispatch (ftControl, msg);
}

void MCOMMTOX::cb_file_chunk_request (uint32_t friend_number,
                                      uint32_t file_number, uint64_t position,
                                      size_t length)
{
    printf ("FILE CHUNK REQUEST: %d %d\n", position, length);
    PBMessage_t * msg = PB_New_Message ();

    msg->I1 = friend_number;
    msg->I2 = file_number;
    msg->I3 = position;
    msg->I4 = length;
    xwfDispatch (ftBytesRequest, msg);
    return;
}