/* Why are all strings in an event allocated some memory?
   Because SunRPC expects all strings in a struct to be allocated (and static
   allocations won't work).
   SunRPC will free these extra strings. */

#include <threads.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <tox/tox.h>

#include "list.h"
#include "misc.h"
#include "hexstring.h"

#include "toxaemia_core.h"

extern short F_online[65535];
extern List_t *Groupchats;

void cb_self_connection_status(Tox *tox, TOX_CONNECTION connection_status,
                               void *user_data)
{
	char *status;

	if(connection_status == TOX_CONNECTION_NONE) status ="Disconnected";
	else if(connection_status == TOX_CONNECTION_TCP) status ="TCP";
	else if(connection_status == TOX_CONNECTION_UDP) status ="UDP";
	else status ="Unknown";

	mtx_lock(&Tox_comm->ConnectedMtx);
	Tox_comm->Connected =1;
	cnd_broadcast(&Tox_comm->ConnectedCnd);
	mtx_unlock(&Tox_comm->ConnectedMtx);

	dbg("Connection status changed: %s\n", status);
}

void cb_friend_connection_status(Tox *tox, uint32_t friend_number,
                                 TOX_CONNECTION connection_status,
                                 void *user_data)
{
	ToxEvent_t *event = calloc(1, sizeof(ToxEvent_t));

	event->type =FCONN;
	event->paramid =friend_number;

	if(connection_status == TOX_CONNECTION_NONE) event->param0 =0;
	else if(connection_status == TOX_CONNECTION_TCP) event->param0 =1;
	else if(connection_status == TOX_CONNECTION_UDP) event->param0 =2;

	F_online[friend_number] =event->param0;

	event->paramid =friend_number;
	event->param1 =calloc(1, sizeof(char));
	event->param2.param2_len =0;
	event->param3.param3_len =0;

	dbg("ID %d status %d\n", friend_number, connection_status);

	List_add(Events, event);

}

void cb_friend_name(Tox *tox, uint32_t friend_number, const uint8_t *name,
                    size_t length, void *user_data)
{
	char *nname =calloc(length + 1, sizeof(char));
	ToxEvent_t *event = calloc(1, sizeof(ToxEvent_t));

	event->type =FNAME;
	event->paramid =friend_number;
	event->param1 =nname;
	event->param2.param2_len =0;
	event->param3.param3_len =0;

	strncpy(nname, (char*) name, length);
	nname[length+1] ='\0';
	List_add(Events, event);
}

void cb_friend_status_message(Tox *tox, uint32_t friend_number,
                              const uint8_t *status, size_t length,
                              void *user_data)
{
	char *nstatus =calloc(length + 1, sizeof(char));
	ToxEvent_t *event =calloc(1, sizeof(ToxEvent_t));

	event->type =FSTATUS;
	event->paramid =friend_number;
	event->param1 =nstatus;
	event->param2.param2_len =0;
	event->param3.param3_len =0;

	strncpy(nstatus, (char*) status, length);
	nstatus[length+1] ='\0';
	List_add(Events, event);
}

void cb_friend_request(Tox *tox, const uint8_t *public_key,
                       const uint8_t *message, size_t length, void *user_data)
{
	char *nbundle =calloc((TOX_PUBLIC_KEY_SIZE * 2) + length + 3, sizeof(char));
	char *s_message =strndup((const char*) message, length);
	char *s_pubkey =bin_to_hex_string(public_key, TOX_PUBLIC_KEY_SIZE);
	ToxEvent_t *event =calloc(1, sizeof(ToxEvent_t));

	event->type =FREQUEST;
	event->param1 =nbundle;
	event->param2.param2_len =0;
	event->param3.param3_len =0;

	sprintf(nbundle, "%s %s", s_pubkey, s_message);
	dbg("Friend request: %s", nbundle);
	free (s_message); free(s_pubkey);
	List_add(Events, event);
}

void cb_friend_message(Tox *tox, uint32_t friend_number, TOX_MESSAGE_TYPE type,
                       const uint8_t *message, size_t length, void *user_data)
{
	char *nmessage =calloc(length + 1, sizeof(char));

	ToxEvent_t *event = calloc(1, sizeof(ToxEvent_t));
	strncpy(nmessage, (char*) message, length);
	nmessage[length+1] ='\0';

	event->type =FMESSAGE;
	event->paramid =friend_number;
	event->param1 =nmessage;
	event->param2.param2_len =0;
	event->param3.param3_len =0;

	dbg("Message from ID %d: %s\n", event->paramid, nmessage);
	List_add(Events, event);
}

void cb_group_invite(Tox *tox, int32_t friendnumber, uint8_t type,
                     const uint8_t *data, uint16_t length, void * user_data)
{
	unsigned int gid =tox_join_groupchat(tox, friendnumber, data, length);

	if(gid != -1)
	{
		ToxEvent_t *event = calloc(1, sizeof(ToxEvent_t));
		List_add(Groupchats, (void*) gid);
		dbg("Joined groupchat %d\n", gid);
		event->type =GNEW;
		event->paramid =gid;
		event->param1 =calloc(1, sizeof(char));
		event->param2.param2_len =0;
		event->param3.param3_len =0;
		List_add(Events, event);
	}
}

void cb_group_namelist_change(Tox *tox, int groupnum, int peernum,
                              uint8_t change, void* user_data)
{
	unsigned short pos =0, gpos =0;
	unsigned short numpeers =tox_group_number_peers(tox, groupnum);

	if(numpeers < 1)
	{
		dbg("fail: numpeers < 1\n");
		return;
	}

	unsigned short peerlens[numpeers];
	char peernames[numpeers][TOX_MAX_NAME_LENGTH];
	char *gnames =calloc((numpeers * 2) + 1 + (numpeers * TOX_MAX_NAME_LENGTH),
	                     sizeof(char));

	if(tox_group_get_names(tox, groupnum, (uint8_t(*)[128])peernames, peerlens,
	                       numpeers) == -1)
	{
		dbg("fail\n");
		free(gnames);
		return;
	}


	for(int i =0; i < numpeers; i++)
	{
		char *tmpname =calloc(TOX_MAX_NAME_LENGTH, sizeof(char));
		memcpy(tmpname, peernames[pos], peerlens[i]);
		pos +=1;

		if(i < (numpeers - 1)) gpos +=sprintf(gnames+gpos, "%s, ", tmpname);
		else gpos +=sprintf(gnames+gpos, "%s", tmpname);
		free (tmpname);
	}

	ToxEvent_t *event =calloc(1, sizeof(ToxEvent_t));
	event->type =GNAMES;
	event->paramid =groupnum;
	event->param1 =gnames;
	event->param2.param2_len =numpeers * TOX_MAX_NAME_LENGTH;
	event->param2.param2_val =calloc((numpeers * TOX_MAX_NAME_LENGTH),
	                                 sizeof(char));
	memcpy(event->param2.param2_val, peernames, (numpeers * TOX_MAX_NAME_LENGTH));
	event->param3.param3_val =calloc(numpeers, sizeof(short));
	memcpy(event->param3.param3_val, peerlens, sizeof(short) * numpeers);
	event->param3.param3_len =numpeers;
	List_add(Events, event);
}

void cb_group_message(Tox *tox, int groupnumber, int peernumber,
                      const uint8_t * message, uint16_t length, void *userdata)
{
	char *msg =strndup((const char*)message, length);
	ToxEvent_t *event = calloc(1, sizeof(ToxEvent_t));

	event->type =GMESSAGE;
	event->paramid =groupnumber;
	event->param0 =peernumber;
	event->param1 =msg;
	event->param2.param2_len =0;
	event->param3.param3_len =0;

	dbg("Group message: %s\n", msg);
	List_add(Events, event);
}

void InitCallbacks()
{
	tox_callback_self_connection_status(Tox_comm->tox,
	                                    cb_self_connection_status, 0);

	tox_callback_friend_connection_status(Tox_comm->tox,
	                                      cb_friend_connection_status, 0);
	tox_callback_friend_name(Tox_comm->tox, cb_friend_name, 0);
	tox_callback_friend_status_message(Tox_comm->tox, cb_friend_status_message,
	                                   0);
	tox_callback_friend_message(Tox_comm->tox, cb_friend_message, 0);
	tox_callback_friend_request(Tox_comm->tox, cb_friend_request, 0);

	tox_callback_group_invite(Tox_comm->tox, cb_group_invite, 0);
	tox_callback_group_namelist_change(Tox_comm->tox, cb_group_namelist_change,
	                                   0);
	tox_callback_group_message(Tox_comm->tox, cb_group_message, 0);
}
