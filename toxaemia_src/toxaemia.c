#include <threads.h>
#include <stdio.h> // sscanf()
#include <stdlib.h> // calloc()
#include <string.h> // strdup()
#include <unistd.h> // usleep()

#include <tox/tox.h>

#include "misc.h"
#include "list.h"

#include "toxaemia_core.h"

Tox_comm_t *Tox_comm;
int Tox_thread_launched =0;

int Tox_comm_main();

uint8_t* hex_string_to_bin(const char *hex_string) 
{
	size_t i, len = strlen(hex_string) / 2;
	uint8_t *ret = calloc(len, 1);

	for(i = 0; i < len; i++) 
	{
		sscanf(hex_string + i * 2, "%2hhx", &ret[i]);
	}

	return ret;
}

int launch_tox_thread()
{
	printf("Launching Tox thread\n");
	cnd_init(&Tox_comm->ConnectedCnd);
	mtx_init(&Tox_comm->ConnectedMtx, mtx_plain);

	thrd_create (&Tox_comm->Thrd, Tox_comm_main, 0);

	mtx_lock (&Tox_comm->ConnectedMtx);

	while(!Tox_comm->Connected) 
		{ cnd_wait(&Tox_comm->ConnectedCnd, &Tox_comm->ConnectedMtx); }

	mtx_unlock (&Tox_comm->ConnectedMtx);

	List_add(&Tox_comm->ICQueue, strdup("Hello"));

	return 0;
}

int Tox_comm_main()
{
	TOX_ERR_OPTIONS_NEW toxoptserr;
	TOX_ERR_NEW toxerr;

	struct Tox_Options *Topts =tox_options_new(&toxoptserr);

	Topts->ipv6_enabled =false;
	
	Tox_comm->tox =tox_new(Topts, &toxerr);

	tox_self_set_name(Tox_comm->tox, (uint8_t*)Tox_comm->Name, strlen(Tox_comm->Name), NULL); // Sets the username
    tox_self_set_status_message(Tox_comm->tox, (uint8_t*)Tox_comm->Status, strlen(Tox_comm->Status), NULL); // Sets the status message

    tox_self_set_status(Tox_comm->tox, TOX_USER_STATUS_NONE);

    tox_bootstrap(Tox_comm->tox, Tox_comm->BootstrapAddress, Tox_comm->BootstrapPort, hex_string_to_bin(Tox_comm->BootstrapKey), NULL);

	mtx_lock (&Tox_comm->ConnectedMtx);
	Tox_comm->Connected =1;
	cnd_broadcast(&Tox_comm->ConnectedCnd);
	mtx_unlock (&Tox_comm->ConnectedMtx);

	while (1)
	{
		if(Tox_comm->ICQueue)
		{
			dbg("Recieved: %s", 
					List_retrieve_and_remove_first(&Tox_comm->ICQueue));
		}

		tox_iterate(Tox_comm->tox);

		usleep(tox_iteration_interval(Tox_comm->tox) * 1000);
	}
	return 0;
}
	

int*
toxconnect_1_svc(int BootstrapPort, char* BootstrapAddress, char* BootstrapKey,
				char* Name, char* Status, struct svc_req* SvcReq)
{
	static int result;
	if (!Tox_comm) Tox_comm =calloc(1, sizeof(Tox_comm_t));

	Tox_comm->BootstrapPort =BootstrapPort;
	Tox_comm->BootstrapAddress =strdup(BootstrapAddress);
	Tox_comm->BootstrapKey =strdup(BootstrapKey);
	Tox_comm->Name =strdup(Name);
	Tox_comm->Status =strdup(Status);


	if(!Tox_thread_launched) launch_tox_thread();

	return &result;
}
