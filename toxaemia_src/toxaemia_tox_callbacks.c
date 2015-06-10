#include <threads.h>

#include <tox/tox.h>

#include "toxaemia_core.h"

void cb_self_connection_status(Tox *tox, TOX_CONNECTION connection_status, 
void *user_data)
{
	char *status;

	if (connection_status == TOX_CONNECTION_NONE) status ="Disconnected";
	else if (connection_status == TOX_CONNECTION_TCP) status ="TCP";
	else if (connection_status == TOX_CONNECTION_UDP) status ="UDP";

	mtx_lock (&Tox_comm->ConnectedMtx);
	Tox_comm->Connected =1;
	cnd_broadcast(&Tox_comm->ConnectedCnd);
	mtx_unlock (&Tox_comm->ConnectedMtx);

	dbg("Connection status changed: %s\n", status);
}
void InitCallbacks()
{
	tox_callback_self_connection_status(Tox_comm->tox, cb_self_connection_status, 0);	
}

