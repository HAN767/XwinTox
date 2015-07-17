#include <string.h>

#include "MCOMMTOX.h"
#include "xwintox.h"

void MCOMMTOX::recvSignal(unsigned int dwType, PBMessage_t* msg)
{
	switch (dwType)
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
	}
}
