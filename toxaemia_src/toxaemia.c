#include <threads.h>
#include <stdio.h> // sscanf()
#include <stdlib.h> // calloc()
#include <string.h> // strdup()
#include <unistd.h> // usleep()

#include <tox/tox.h>

#include "misc.h"
#include "list.h"

#include "toxaemia_rpc.h"
#include "toxaemia_core.h"
#include "toxaemia_util.h"

Tox_comm_t *Tox_comm;
int Tox_thread_launched =0;

short F_online[65535] = { 0 };

List_t *Groupchats =0;

void InitCallbacks();

int Tox_comm_main();

int launch_tox_thread()
{
	dbg("Launching Tox thread\n");
	cnd_init(&Tox_comm->ConnectedCnd);
	mtx_init(&Tox_comm->ConnectedMtx, mtx_plain);

	thrd_create(&Tox_comm->Thrd, Tox_comm_main, 0);

	mtx_lock(&Tox_comm->ConnectedMtx);
	Tox_thread_launched =1;

	mtx_unlock(&Tox_comm->ConnectedMtx);

	return 0;
}

void Connect_to_tox()
{
	TOX_ERR_BOOTSTRAP toxberr;
	tox_self_set_name(Tox_comm->tox, (uint8_t*)Tox_comm->Name,
	                  strlen(Tox_comm->Name), NULL);
	tox_self_set_status_message(Tox_comm->tox, (uint8_t*)Tox_comm->Status,
	                            strlen(Tox_comm->Status), NULL);

	tox_self_set_status(Tox_comm->tox, TOX_USER_STATUS_NONE);

	tox_bootstrap(Tox_comm->tox, Tox_comm->BootstrapAddress,
	              Tox_comm->BootstrapPort,hex_string_to_bin(Tox_comm->BootstrapKey),&toxberr);

	if(toxberr != TOX_ERR_BOOTSTRAP_OK)
	{
		dbg("Failed to bootstrap\n");
	}
}

void Deliver_save_data()
{
	mtx_lock(&Tox_comm->SaveDataMtx);
	Tox_comm->SaveData.Data.Data_len =tox_get_savedata_size(Tox_comm->tox);
	Tox_comm->SaveData.Data.Data_val =calloc(Tox_comm->SaveData.Data.Data_len+1,
	                                  sizeof(char));
	tox_get_savedata(Tox_comm->tox,
	                 (uint8_t *) Tox_comm->SaveData.Data.Data_val);
	Tox_comm->SaveData.Data.Data_val[Tox_comm->SaveData.Data.Data_len+1] = 'F';
	Tox_comm->SaveData.Data.Data_len++;
	dbg("Save Length: %d\n", Tox_comm->SaveData.Data.Data_len);
	mtx_unlock(&Tox_comm->SaveDataMtx);

}

void Send_friend_request(char* id, char* message)
{
	TOX_ERR_FRIEND_ADD err;
	int newid;
	uint8_t* binid =hex_string_to_bin(id);

	newid =tox_friend_add(Tox_comm->tox, binid, (uint8_t *) message,
	                      strlen(message), &err);
	dbg("ID %s, Msg %s, Error %d\n", id, message, err);

	if(err == TOX_ERR_FRIEND_ADD_OK) List_add(&Returns, (void*)newid);
	else List_add(&Returns, (void*)-1);

}

void Deliver_friend_list()
{
	unsigned int *data;

	List_add(&Returns, (void*) tox_self_get_friend_list_size(Tox_comm->tox));

	data =calloc((int) Returns->data, sizeof(unsigned int));
	tox_self_get_friend_list(Tox_comm->tox, data);
	List_add(&Returns, data);
}

void Deliver_friend(unsigned int num)
{
	char *name, *statusm;
	int status;
	unsigned char *pubkey;

	TOX_ERR_FRIEND_QUERY err;
	TOX_ERR_FRIEND_GET_PUBLIC_KEY errp;

	size_t size = tox_friend_get_name_size(Tox_comm->tox, num, 0) + 1;
	name =calloc(size, sizeof(char));
	tox_friend_get_name(Tox_comm->tox, num, (uint8_t*)name, 0);
	name[size+1] ='\0';

	size = tox_friend_get_status_message_size(Tox_comm->tox, num, 0) + 1;
	statusm =calloc(size, sizeof(char));
	tox_friend_get_status_message(Tox_comm->tox, num, (uint8_t*)statusm, 0);
	statusm[size+1] ='\0';

	pubkey =calloc(TOX_PUBLIC_KEY_SIZE, sizeof(unsigned char));
	tox_friend_get_public_key(Tox_comm->tox, num, pubkey, &errp);

	status =tox_friend_get_status(Tox_comm->tox, num, &err);

	List_add(&Returns, name);
	List_add(&Returns, statusm);
	List_add(&Returns, bin_to_hex_string(pubkey, TOX_PUBLIC_KEY_SIZE));
	List_add(&Returns, (void*)status);
	List_add(&Returns, (void*) F_online[num]);
}

void Send_message(ToxMessageType type, unsigned int num, char* message)
{
	if(type == MFRIEND)
	{
		tox_friend_send_message(Tox_comm->tox, num, TOX_MESSAGE_TYPE_NORMAL,
		                        (uint8_t*) message, strlen(message), 0);
	}
	else
	{
		tox_group_message_send(Tox_comm->tox, num, (uint8_t*) message,
		                       strlen(message));
	}
}

void Create_group_chat()
{
	List_add(&Returns, (void*)tox_add_groupchat(Tox_comm->tox));
}

/* new call structure */

void Set_name_status(char* name, char* status)
{
	dbg("Set name or status: %s, %s\n", name, status);
	tox_self_set_name(Tox_comm->tox, (uint8_t*)name, strlen(name), 0);
	tox_self_set_status_message(Tox_comm->tox, (uint8_t*)
	                            status, strlen(status), 0);
}

int Tox_comm_main()
{
	TOX_ERR_OPTIONS_NEW toxoptserr;
	TOX_ERR_NEW toxerr;

	struct Tox_Options *Topts =tox_options_new(&toxoptserr);

	Topts->ipv6_enabled =false;

	if(Tox_comm->SaveData.Data.Data_len != 0)
	{
		Topts->savedata_type =TOX_SAVEDATA_TYPE_TOX_SAVE;
		Topts->savedata_data =(uint8_t*) Tox_comm->SaveData.Data.Data_val;
		Topts->savedata_length =Tox_comm->SaveData.Data.Data_len;
		dbg("Savedata length: %d\n", Topts->savedata_length);
	}

	Tox_comm->tox =tox_new(Topts, &toxerr);

	if(toxerr != TOX_ERR_NEW_OK)
	{
		char *errcode;

		if(toxerr == TOX_ERR_NEW_LOAD_BAD_FORMAT) errcode ="Bad save format";
		else errcode ="Unknown";

		dbg("Failed to create new Tox: %s\n", errcode);
		return -1;
	}

	InitCallbacks();


	while(1)
	{
		if(Tox_comm->ICQueue)
		{
			char* Rmsg =List_retrieve_and_remove_first(&Tox_comm->ICQueue);
			char* tofree =Rmsg;

			if(!strcmp(Rmsg, "getsavedata"))	Deliver_save_data();
			else if(!strncmp(Rmsg, "sendfriendrequest", 17))
			{
				char *id;
				strsep(&Rmsg, " ");
				id =Rmsg;
				strsep(&Rmsg, " ");
				Send_friend_request(id, Rmsg);
			}
			else if(!strcmp(Rmsg, "getfriendlist"))	Deliver_friend_list();
			else if(!strncmp(Rmsg, "getfriend", 9))
			{
				strsep(&Rmsg, " ");
				Deliver_friend(strtol(Rmsg, 0, 10));
			}
			else if(!strncmp(Rmsg, "connect", 7)) Connect_to_tox();
			else if(!strncmp(Rmsg, "sendmessage", 11))
			{
				char *type, *id;
				strsep(&Rmsg, " ");
				type =Rmsg;
				strsep(&Rmsg, " ");
				id =Rmsg;
				strsep(&Rmsg, " ");
				Send_message(strtol(type, 0, 10), strtol(id, 0, 10), Rmsg);
			}
			else if(!strcmp(Rmsg, "creategroupchat")) Create_group_chat();
			else
			{
				dbg("Unhandled request %s\n", Rmsg);
			}

			free(tofree);
		}

		if(Calls)
		{
			Call_t *call =List_retrieve_and_remove_first(&Calls);

			if(call->Func ==ToxSetNameStatus)
			{
				Set_name_status(call->S1, call->S2);
			}

			if(call->S1) free(call->S1);

			if(call->S2) free(call->S2);
			
			free(call);
		}

		tox_iterate(Tox_comm->tox);

		usleep(tox_iteration_interval(Tox_comm->tox) * 1000);
	}

	return 0;
}
