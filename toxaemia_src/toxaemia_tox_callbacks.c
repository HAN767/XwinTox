#include <threads.h>
#include <stdlib.h>
#include <string.h>

#include <tox/tox.h>

#include "toxaemia_core.h"

void cb_self_connection_status(Tox *tox, TOX_CONNECTION connection_status, 
							   void *user_data)
{
	char *status;

	if (connection_status == TOX_CONNECTION_NONE) status ="Disconnected";
	else if (connection_status == TOX_CONNECTION_TCP) status ="TCP";
	else if (connection_status == TOX_CONNECTION_UDP) status ="UDP";
	else status ="Unknown";

	mtx_lock (&Tox_comm->ConnectedMtx);
	Tox_comm->Connected =1;
	cnd_broadcast(&Tox_comm->ConnectedCnd);
	mtx_unlock (&Tox_comm->ConnectedMtx);

	dbg("Connection status changed: %s\n", status);
}

void cb_friend_connection_status(Tox *tox, uint32_t friend_number, 
								 TOX_CONNECTION connection_status,
								 void *user_data)
{
	const char *status;
	ToxEvent_t *event = calloc(1, sizeof(ToxEvent_t));

	event->type =FCONN;

	if (connection_status == TOX_CONNECTION_NONE) event->param0 =0;
	else if (connection_status == TOX_CONNECTION_TCP) event->param0 =1;
	else if (connection_status == TOX_CONNECTION_UDP) event->param0 =2;

	event->param1 =" "; event->param2 =" "; event->param3 =" ";

	List_add(&Events, event);

}

void cb_friend_name(Tox *tox, uint32_t friend_number, const uint8_t *name,
					size_t length, void *user_data)
{
	char *nname =calloc(length + 1, sizeof(char));
	ToxEvent_t *event = calloc(1, sizeof(ToxEvent_t));

	event->type =FNAME;
	event->param1 =nname;

	strncpy(nname, (char*) name, length); nname[length+1] ='\0';
	List_add(&Events, event);
}

void InitCallbacks()
{
	tox_callback_self_connection_status(Tox_comm->tox, 
										cb_self_connection_status, 0);	
	tox_callback_friend_connection_status(Tox_comm->tox, 
										  cb_friend_connection_status, 0);	
	tox_callback_friend_name(Tox_comm->tox, cb_friend_name, 0);
}

