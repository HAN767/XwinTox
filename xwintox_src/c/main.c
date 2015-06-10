#include <stdio.h> // printf()
#include <stdlib.h> // calloc()
#include <string.h> // strdup()
#include <sys/stat.h> // umask()
#include <threads.h> // threading

#include "misc.h"
#include "list.h"
#include "dictionary.h"

#include "toxaemia_rpc.h"

#include "xwintox.h"
#include "etc.h"

XwinTox_instance_t *APP;

extern int CXXMain();

int main()
{
	CLIENT *clnt;

	/* component independent main */
	APP =calloc(1, sizeof (XwinTox_instance_t));
	APP->Comm =calloc(1, sizeof(Comm_t));
	APP->Xwin =calloc(1, sizeof(Xwin_t));

	APP->Config =Dictionary_new(24);
	APP->ConfigFilename =get_config_filename();

	umask(S_IRGRP | S_IWGRP | S_IROTH | S_IWOTH);

	create_config_folder();
	if (Dictionary_load_from_file(APP->Config, 
		APP->ConfigFilename, 1)) 
	{ default_config(APP->Config); }

	clnt = clnt_create("localhost", TOXAEMIA_PROG, TOXAEMIA_VERS1, "udp");
	if (clnt == (CLIENT *) NULL) 
	{
		clnt_pcreateerror("localhost");
		exit(1);
	}

	cnd_init(&APP->Comm->WorkCnd);
	mtx_init(&APP->Comm->WorkMtx, mtx_plain);
	thrd_create (&APP->Xwin->Thrd, CXXMain, 0);


#define GTC(X) (char *)Dictionary_get (APP->Config, X )

	APP->Connected = !toxconnect_1( 
					(int)atoi(Dictionary_get(APP->Config, "Tox.BootstrapPort")), 
					GTC("Tox.BootstrapIP"), GTC("Tox.BootstrapKey"), 
					GTC("Tox.Name"), GTC("Tox.Status") , clnt);

	while (!APP->Comm->WantQuit)
	{
		mtx_lock(&APP->Comm->WorkMtx);

		while(!APP->Comm->Work) 
			{ cnd_wait(&APP->Comm->WorkCnd, &APP->Comm->WorkMtx); }
		APP->Comm->WorkMtx =0;

		mtx_unlock(&APP->Comm->WorkMtx);
	}
	
	Dictionary_write_to_file(APP->Config, APP->ConfigFilename);

	return 0;
}
