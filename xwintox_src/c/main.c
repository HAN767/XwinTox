#include <stdio.h> // printf()
#include <stdlib.h> // calloc()
#include <string.h> // strdup()
#include <sys/stat.h> // umask()
#include <threads.h> // threading

#include "misc.h"
#include "list.h"
#include "dictionary.h"

#include "toxaemia_rpc.h"

#include "etc.h"

typedef struct Tox_comm_s
{
	thrd_t Thread;
	List_t* ICQueue;
} Tox_comm_t;

typedef struct XwinTox_instance_s
{
	Dictionary_t *Config;
	Tox_comm_t *ToxComm;
	char* ConfigFilename;
} XwinTox_instance_t;

XwinTox_instance_t *Instance;

int main()
{
	int conres;
	CLIENT *clnt;

	/* component independent main */
	Instance =calloc(1, sizeof (XwinTox_instance_t));
	Instance->Config =Dictionary_new(24);
	Instance->ConfigFilename =get_config_filename();

	umask(S_IRGRP | S_IWGRP | S_IROTH | S_IWOTH);

	create_config_folder();
	if (Dictionary_load_from_file(Instance->Config, 
		Instance->ConfigFilename, 1)) 
	{ default_config(Instance->Config); }

	clnt = clnt_create("localhost", TOXAEMIA_PROG, TOXAEMIA_VERS1, "udp");
	if (clnt == (CLIENT *) NULL) 
	{
		clnt_pcreateerror("localhost");
		exit(1);
	}

#define GTC(X) (char *)Dictionary_get (Instance->Config, X )

	conres = toxconnect_1( atoi(Dictionary_get(Instance->Config, "Tox.BootstrapPort")), GTC("Tox.BootstrapIP"), GTC("Tox.BootstrapKey"), GTC("Tox.Name"), GTC("Tox.Status") , clnt);
	
	Dictionary_write_to_file(Instance->Config, Instance->ConfigFilename);

	return 0;
}
