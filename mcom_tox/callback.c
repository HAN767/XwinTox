#include <tox/tox.h>

#include "misc.h"
#include "postbox.h"
#include "xwintox.h"

#include "MCOMMTOX.h"
#include "callback.h"

void register_callbacks(XWF_Object_Handle_t *hobjSelf)
{
	PREP
	tox_callback_self_connection_status(TOXINST, cb_self_connection_status, hobjSelf);
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
