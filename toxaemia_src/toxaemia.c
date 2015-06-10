#include <threads.h>
#include <stdio.h> // sscanf()
#include <stdlib.h> // calloc()
#include <string.h> // strdup()
#include <unistd.h> // usleep()

#include <tox/tox.h>

#include "misc.h"
#include "list.h"

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

	while(!Tox_comm->Connected) 
		{ err =cnd_wait(&Tox_comm->ConnectedCnd, &Tox_comm->ConnectedMtx); }

	mtx_unlock (&Tox_comm->ConnectedMtx);

	if (err != thrd_success) { dbg("Failed to connect to Tox\n"); return 1; }

	return 0;
}

int Tox_comm_main()
{
	TOX_ERR_OPTIONS_NEW toxoptserr;
	TOX_ERR_NEW toxerr;
	TOX_ERR_BOOTSTRAP toxberr;

	struct Tox_Options *Topts =tox_options_new(&toxoptserr);

	Topts->ipv6_enabled =false;
	
	Tox_comm->tox =tox_new(Topts, &toxerr);

	InitCallbacks();

	tox_self_set_name(Tox_comm->tox, (uint8_t*)Tox_comm->Name, 
					  strlen(Tox_comm->Name), NULL);
    tox_self_set_status_message(Tox_comm->tox, (uint8_t*)Tox_comm->Status, 
								strlen(Tox_comm->Status), NULL);

    tox_self_set_status(Tox_comm->tox, TOX_USER_STATUS_NONE);

    tox_bootstrap(Tox_comm->tox, Tox_comm->BootstrapAddress, 
	Tox_comm->BootstrapPort,hex_string_to_bin(Tox_comm->BootstrapKey),&toxberr);

	if (toxerr != TOX_ERR_BOOTSTRAP_OK) 
	{
		dbg("Failed to bootstrap\n");
		return -1;
	}

	while (1)
	{
		if(Tox_comm->ICQueue)
		{
			char* Rmsg =List_retrieve_and_remove_first(&Tox_comm->ICQueue);
			dbg("Recieved: %s", Rmsg);
		}

		tox_iterate(Tox_comm->tox);

		usleep(tox_iteration_interval(Tox_comm->tox) * 1000);
	}
	return 0;
}
