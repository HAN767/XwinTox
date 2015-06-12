#include <stdio.h> // printf()
#include <stdlib.h> // calloc()
#include <string.h> // strdup()
#include <sys/stat.h> // umask()
#include <threads.h> // threading
#include <unistd.h> // sleeping

#include "misc.h"
#include "list.h"
#include "dictionary.h"

#include "toxaemia_rpc.h"

#include "xwintox.h"
#include "etc.h"

#define GTC(X) (char *)Dictionary_get (APP->Config, X )

XwinTox_instance_t *APP;
CLIENT *clnt;

extern int CXXMain();

int savedata()
{
	FILE *save;
	char *filename =get_save_filename();
	ToxSaveData_t *savedata;

	save =fopen(filename, "wb");
	if (save == NULL) {dbg("Failed to open savefile %s\n", filename); return 1;}

	savedata =toxgetsavedata_1(clnt);
	if (!savedata)
	{
		clnt_perror(clnt, "Savedata"); fclose(save);
		return 1;
	}
	dbg("Len: %d\n", savedata->Data.Data_len);

	if (fwrite(savedata->Data.Data_val, savedata->Data.Data_len, 1, save) !=1) 
	{
		dbg("Failed to save data to savefile %s\n", filename); fclose(save);
		return 1;
	}

	dbg("Saved Tox data to %s", filename);

	fclose (save);
	return 0;
}
	
int loaddata()
{
	FILE *save;
	struct stat st;
	off_t length;
	char *filename =get_save_filename();
	static ToxSaveData_t savedata;

	save =fopen(filename, "rb");
	if (save == NULL) {dbg("Failed to open savefile %s\n", filename); return 1;}

	stat(filename, &st);
	length =st.st_size;
	if (length == 0) 
	{
		fclose(save); dbg ("Save file unusual: length is zero\n"); return 1;
	}

	unsigned char data[length];
	fread(data, sizeof(data), 1, save);

	savedata.Data.Data_len =length;
	savedata.Data.Data_val =data;

	toxinstallsavedata_1(savedata, clnt);
	return 0;
}


int main()
{
	struct rpc_err rpcerr;

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

	loaddata();
	APP->Connected = !toxconnect_1( 
					(int)atoi(Dictionary_get(APP->Config, "Tox.BootstrapPort")), 
					GTC("Tox.BootstrapIP"), GTC("Tox.BootstrapKey"), 
					GTC("Tox.Name"), GTC("Tox.Status") , clnt);

	sleep(1); savedata();

	while (!APP->Comm->WantQuit)
	{
		void* work =0;

		mtx_lock(&APP->Comm->WorkMtx);

		while(!APP->Comm->Work) 
			{ cnd_wait(&APP->Comm->WorkCnd, &APP->Comm->WorkMtx); }
		APP->Comm->WorkMtx =0;

		work =List_retrieve_and_remove_first(&APP->Comm->WorkQueue);
		if(!work) goto nowork;

		if(strcmp (work, "savedata") == 0) { savedata(); }

		nowork:
		mtx_unlock(&APP->Comm->WorkMtx);
	}
	
	Dictionary_write_to_file(APP->Config, APP->ConfigFilename);

	return 0;
}
