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

void InitCallbacks();

int Tox_comm_main();

int launch_tox_thread()
{
	int err;

	dbg("Launching Tox thread\n");
	cnd_init(&Tox_comm->ConnectedCnd);
	mtx_init(&Tox_comm->ConnectedMtx, mtx_plain);

	thrd_create (&Tox_comm->Thrd, Tox_comm_main, 0);

	mtx_lock (&Tox_comm->ConnectedMtx);
	Tox_thread_launched =1;

	while(!Tox_comm->Connected) 
		{ err =cnd_wait(&Tox_comm->ConnectedCnd, &Tox_comm->ConnectedMtx); }

	mtx_unlock (&Tox_comm->ConnectedMtx);

	if (err != thrd_success) { dbg("Failed to connect to Tox\n"); return 1; }

	return 0;
}

void Deliver_save_data()
{
	mtx_lock (&Tox_comm->SaveDataMtx);
	Tox_comm->SaveData.Data.Data_len =tox_get_savedata_size(Tox_comm->tox);
	dbg("Save Length: %d\n", Tox_comm->SaveData.Data.Data_len);
	Tox_comm->SaveData.Data.Data_val =calloc(Tox_comm->SaveData.Data.Data_len, 
											sizeof(unsigned char));
	tox_get_savedata(Tox_comm->tox, 
					(uint8_t *) Tox_comm->SaveData.Data.Data_val);
	mtx_unlock (&Tox_comm->SaveDataMtx);

}

void Send_friend_request(char* id, char* message)
{
 	TOX_ERR_FRIEND_ADD err;
	uint8_t* binid =hex_string_to_bin(id); 

	
	tox_friend_add(Tox_comm->tox, binid, (uint8_t *) message, 
				  strlen(message), &err);
	dbg("ID %s, Msg %s, Error %d\n", id, message, err);
	
}

void Deliver_friend_list()
{
	unsigned int *data;

	List_add(&Returns, (int) tox_self_get_friend_list_size(Tox_comm->tox));

	data =calloc(Returns->data, sizeof (unsigned int));
	tox_self_get_friend_list(Tox_comm->tox, data);
	List_add(&Returns, data);
}

int Tox_comm_main()
{
	TOX_ERR_OPTIONS_NEW toxoptserr;
	TOX_ERR_NEW toxerr;
	TOX_ERR_BOOTSTRAP toxberr;

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
	if (toxerr != TOX_ERR_NEW_OK)
	{
		char *errcode;
		if (toxerr == TOX_ERR_NEW_LOAD_BAD_FORMAT) errcode ="Bad save format";
		dbg("Failed to create new Tox: %s\n", errcode);
		return -1;
	}

	InitCallbacks();

	tox_self_set_name(Tox_comm->tox, (uint8_t*)Tox_comm->Name, 
					  strlen(Tox_comm->Name), NULL);
    tox_self_set_status_message(Tox_comm->tox, (uint8_t*)Tox_comm->Status, 
								strlen(Tox_comm->Status), NULL);

    tox_self_set_status(Tox_comm->tox, TOX_USER_STATUS_NONE);

    tox_bootstrap(Tox_comm->tox, Tox_comm->BootstrapAddress, 
	Tox_comm->BootstrapPort,hex_string_to_bin(Tox_comm->BootstrapKey),&toxberr);

	if (toxberr != TOX_ERR_BOOTSTRAP_OK) 
	{
		dbg("Failed to bootstrap\n");
		return -1;
	}

	while (1)
	{
		if(Tox_comm->ICQueue)
		{
			char* Rmsg =List_retrieve_and_remove_first(&Tox_comm->ICQueue);
			char* tofree =Rmsg;

			if(!strcmp (Rmsg, "getsavedata"))	Deliver_save_data();
			else if(!strncmp (Rmsg, "sendfriendrequest", 17))
			{
				char *id;
				strsep(&Rmsg, " ");
				id =Rmsg;
				strsep(&Rmsg, " "); 
				Send_friend_request(id, Rmsg);
			}
			else if(!strcmp (Rmsg, "getfriendlist"))	Deliver_friend_list();
			else { 	dbg("Unhandled request %s\n", Rmsg); }
			free(tofree);
		}

		tox_iterate(Tox_comm->tox);

		usleep(tox_iteration_interval(Tox_comm->tox) * 1000);
	}
	return 0;
}
