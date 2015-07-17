#include <cstring>

#include "hexstring.h"
#include "xwintox.h"
#include "MCOMMTOX.h"

#define TOXCALLBACK(func) 	tox_callback_##func \
	(tox_, &thunk<decltype(&MCOMMTOX::cb_##func), &MCOMMTOX::cb_##func>::call, \
	 this);	\

void MCOMMTOX::registerCallbacks_()
{

	TOXCALLBACK(self_connection_status)

	TOXCALLBACK(friend_connection_status)
	TOXCALLBACK(friend_status)
	TOXCALLBACK(friend_name)
	TOXCALLBACK(friend_message)
	TOXCALLBACK(friend_request)

	TOXCALLBACK(file_recv)
	TOXCALLBACK(file_recv_control)
	TOXCALLBACK(file_recv_chunk)
}

void MCOMMTOX::cb_self_connection_status(TOX_CONNECTION connection_status)
{
	unsigned int wCStatus;
	PBMessage_t *msgCStatus =PB_New_Message();

	if(connection_status == TOX_CONNECTION_NONE) wCStatus =0;
	else if(connection_status == TOX_CONNECTION_TCP) wCStatus =1;
	else if(connection_status == TOX_CONNECTION_UDP) wCStatus =1;
	else wCStatus =0;

	dbg("Connection status changed: %d\n", wCStatus);
	msgCStatus->I1 =wCStatus;
	xwfDispatch(clConn, msgCStatus);
}

void MCOMMTOX::cb_friend_connection_status(uint32_t friend_number,
        TOX_CONNECTION connection_status)
{
	unsigned int wCStatus;
	TOX_USER_STATUS tuStatus;
	PBMessage_t *msgCStatus =PB_New_Message();

	if(connection_status == TOX_CONNECTION_NONE) wCStatus =stOffline;
	else if(connection_status == TOX_CONNECTION_TCP) wCStatus =stOnline;
	else if(connection_status == TOX_CONNECTION_UDP) wCStatus =stOnline;
	else wCStatus =stOffline;

	if(wCStatus ==stOnline)
	{
		tuStatus =tox_friend_get_status(tox_, friend_number, 0);

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
	xwfDispatch(frStatus, msgCStatus);
}

void MCOMMTOX::cb_friend_status(uint32_t friend_number, TOX_USER_STATUS status)
{
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
	xwfDispatch(frStatus, msgCStatus);
}

void MCOMMTOX::cb_friend_name(uint32_t friend_number, const uint8_t *name,
                              size_t length)
{
	PBMessage_t *msgFrName =PB_New_Message();

	msgFrName->I1 =friend_number;
	msgFrName->S1 =strndup((const char *)name, length);
	xwfDispatch(frName, msgFrName);
}

void MCOMMTOX::cb_friend_message(uint32_t friend_number, TOX_MESSAGE_TYPE type,
                                 const uint8_t *message, size_t length)
{
	PBMessage_t *msgFrMsg =PB_New_Message();

	msgFrMsg->I1 =friend_number;
	msgFrMsg->S1 =strndup((const char *)message, length);
	xwfDispatch(frMsg, msgFrMsg);
}

void MCOMMTOX::cb_friend_request(const uint8_t *public_key,
                                 const uint8_t *message, size_t length)
{
	PBMessage_t *msgFrRequest =PB_New_Message();

	msgFrRequest->S1 =bin_to_hex_string(public_key, TOX_PUBLIC_KEY_SIZE);
	msgFrRequest->S2 =strndup((const char *)message, length);

	xwfDispatch(frRequest, msgFrRequest);
}
