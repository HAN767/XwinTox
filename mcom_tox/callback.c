#include <string.h>
#include <tox/tox.h>

#include "misc.h"
#include "postbox.h"
#include "hexstring.h"

#include "xwintox.h"
#include "MCOMMTOX.h"
#include "callback.h"

void register_callbacks(XWF_Object_Handle_t *hobjSelf)
{
	PREP
	tox_callback_self_connection_status(TOXINST, cb_self_connection_status,
	                                    hobjSelf);

	tox_callback_friend_connection_status(TOXINST, cb_friend_connection_status,
	                                      hobjSelf);
	tox_callback_friend_status(TOXINST, cb_friend_status, hobjSelf);
	tox_callback_friend_name(TOXINST, cb_friend_name, hobjSelf);
	tox_callback_friend_message(TOXINST, cb_friend_message, hobjSelf);
	tox_callback_friend_request(TOXINST, cb_friend_request, hobjSelf);
}

CBFUNC(self_connection_status, TOX_CONNECTION connection_status)
{
	CBPREP
	unsigned int wCStatus;
	PBMessage_t *msgCStatus =PB_New_Message();

	if(connection_status == TOX_CONNECTION_NONE) wCStatus =0;
	else if(connection_status == TOX_CONNECTION_TCP) wCStatus =1;
	else if(connection_status == TOX_CONNECTION_UDP) wCStatus =1;
	else wCStatus =0;

	dbg("Connection status changed: %d\n", wCStatus);
	msgCStatus->I1 =wCStatus;
	DISPATCH(clConn, msgCStatus);
}

CBFUNC(friend_connection_status, uint32_t friend_number,
       TOX_CONNECTION connection_status)
{
	CBPREP
	unsigned int wCStatus;
	TOX_USER_STATUS tuStatus;
	PBMessage_t *msgCStatus =PB_New_Message();

	if(connection_status == TOX_CONNECTION_NONE) wCStatus =stOffline;
	else if(connection_status == TOX_CONNECTION_TCP) wCStatus =stOnline;
	else if(connection_status == TOX_CONNECTION_UDP) wCStatus =stOnline;
	else wCStatus =stOffline;

	if(wCStatus ==stOnline)
	{
		tuStatus =tox_friend_get_status(TOXINST, friend_number, 0);

		switch(tuStatus)
		{
		case TOX_USER_STATUS_NONE:
			wCStatus =stOnline;
			break;

		case TOX_USER_STATUS_AWAY:
			wCStatus =stAway;
			break;

		case TOX_USER_STATUS_BUSY:
			wCStatus =stBusy;
			break;
		}
	}

	msgCStatus->I1 =friend_number;
	msgCStatus->I2 =wCStatus;
	DISPATCH(frStatus, msgCStatus);
}

CBFUNC(friend_status, uint32_t friend_number, TOX_USER_STATUS status)
{
	CBPREP
	unsigned int wCStatus;
	PBMessage_t *msgCStatus =PB_New_Message();

	switch(status)
	{
	case TOX_USER_STATUS_NONE:
		wCStatus =stOnline;
		break;

	case TOX_USER_STATUS_AWAY:
		wCStatus =stAway;
		break;

	case TOX_USER_STATUS_BUSY:
		wCStatus =stBusy;
		break;
	}

	msgCStatus->I1 =friend_number;
	msgCStatus->I2 =wCStatus;
	DISPATCH(frStatus, msgCStatus);
}

CBFUNC(friend_name, uint32_t friend_number, const uint8_t *name, size_t length)
{
	CBPREP
	PBMessage_t *msgFrName =PB_New_Message();

	msgFrName->I1 =friend_number;
	msgFrName->S1 =strndup(name, length);
	DISPATCH(frName, msgFrName);
}

CBFUNC(friend_message, uint32_t friend_number, TOX_MESSAGE_TYPE type,
       const uint8_t *message, size_t length)
{
	CBPREP
	PBMessage_t *msgFrMsg =PB_New_Message();

	msgFrMsg->I1 =friend_number;
	msgFrMsg->S1 =strndup(message, length);
	DISPATCH(frMsg, msgFrMsg);
}

CBFUNC(friend_request, const uint8_t *public_key, const uint8_t *message,
       size_t length)
{
	CBPREP
	PBMessage_t *msgFrRequest =PB_New_Message();

	msgFrRequest->S1 =bin_to_hex_string(public_key, TOX_PUBLIC_KEY_SIZE);
	msgFrRequest->S2 =strndup(message, length);

	DISPATCH(frRequest, msgFrRequest);
}
