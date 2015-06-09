#include <stdio.h> // printf()
#include <stdlib.h> // calloc()
#include <sys/stat.h> // umask()
#include <threads.h> // threading

#include "misc.h"
#include "list.h"
#include "dictionary.h"

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
	/* component independent main */
	Instance =calloc(1, sizeof (XwinTox_instance_t));
	Instance->Config =Dictionary_new(2);
	Instance->ConfigFilename =get_config_filename();

	umask(S_IRGRP | S_IWGRP | S_IROTH | S_IWOTH);

	create_config_folder();
	if (Dictionary_load_from_file(Instance->Config, 
		Instance->ConfigFilename, 1)) 
	{ default_config(Instance->Config); }
	
	Dictionary_write_to_file(Instance->Config, Instance->ConfigFilename);

	return 0;
}
