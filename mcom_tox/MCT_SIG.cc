#include <string.h>

#include "MCOMMTOX.h"
#include "xwintox.h"

void MCOMMTOX::recvSignal(unsigned int dwType, PBMessage_t* msg)
{
	switch(dwType)
	{
	case clSaveData:
		saveToxData_();
		break;

	case frSendMsg:
		dbg("Send message to %d\n", msg->I1);
		mtx_lock(&mtxTox_);
		tox_friend_send_message(tox_, msg->I1, TOX_MESSAGE_TYPE_NORMAL,
		                        (uint8_t*) msg->S1, strlen(msg->S1), 0);
		mtx_unlock(&mtxTox_);
		break;

	case frDelete:
		dbg("Delete friend %d\n", msg->I1);
		mtx_lock(&mtxTox_);
		tox_friend_delete(tox_, msg->I1, 0);
		mtx_unlock(&mtxTox_);
		saveToxData_();
		break;

	case frAcceptRequest:
		acceptFriendRequest_(msg->S1);
		saveToxData_();
		break;

	case frSendRequest:
		dbg("Add friend\n");
		addFriend_(msg->S1, msg->S2);
		break;

	case ftResume:
		dbg("Resume transfer\n");
		controlFileTransfer(msg->I1, msg->I2, TC_Resume);
	}
}

void MCOMMTOX::controlFileTransfer(unsigned int dwFID, unsigned int dwTID,
                                   unsigned int eCtrl)
{
	TOX_ERR_FILE_CONTROL err;
	TOX_FILE_CONTROL c =getToxFC(eCtrl);

	mtx_lock(&mtxTox_);
	if(tox_file_control(tox_, dwFID, dwTID, c, &err))
	{
		PBMessage_t *msg =PB_New_Message();

		dbg("success\n");

		msg->I1 =dwFID;
		msg->I2 =dwTID;

		if(c == TOX_FILE_CONTROL_RESUME) msg->I3 =TC_Resume;
		else if(c == TOX_FILE_CONTROL_PAUSE) msg->I3 =TC_Pause;
		else if(c == TOX_FILE_CONTROL_CANCEL) msg->I3 =TC_Cancel;

		xwfDispatch(ftControl, msg);
	}
	mtx_unlock(&mtxTox_);
}

TOX_FILE_CONTROL MCOMMTOX::getToxFC(unsigned int eCtrl)
{
	if(eCtrl == TC_Resume) return TOX_FILE_CONTROL_RESUME;
	else if (eCtrl == TC_Pause) return TOX_FILE_CONTROL_PAUSE;
	else if (eCtrl == TC_Cancel) return TOX_FILE_CONTROL_CANCEL;
}