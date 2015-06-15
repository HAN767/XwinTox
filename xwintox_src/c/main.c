#include <stdio.h> // printf()
#include <stdlib.h> // calloc()
#include <string.h> // strdup()
#include <sys/stat.h> // umask()
#include <sys/time.h> // gettimeofday()
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

int sendfriendrequest(char* Rmsg)
{
	char *id;
	char *tofree =Rmsg;
	int result;
	
	strsep(&Rmsg, " ");
	id =Rmsg;
	strsep(&Rmsg, " "); 

	/* try to add tox ID */
	if (strlen(id) == 72) 
	{
	dbg("Adding regular Tox ID\n");
	}

	if (toxsendfriendrequest_1(id, Rmsg, clnt))
	{
		clnt_perror(clnt, "Sendfriendrequest");
		return 1;
	}
	return 0;
}
	

int savedata()
{
	FILE *save;
	char *filename =get_save_filename();
	ToxSaveData_t *savedata;

	savedata =toxgetsavedata_1(clnt);
	if (!savedata)
	{
		clnt_perror(clnt, "Savedata");
		return 1;
	}

	unlink(filename);
	save =fopen(filename, "wb");
	if (save == NULL) {dbg("Failed to open savefile %s\n", filename); return 1;}

	if (fwrite(savedata->Data.Data_val, savedata->Data.Data_len-1, 1, save) !=1) 
	{
		dbg("Failed to save data to savefile %s\n", filename); fclose(save);
		return 1;
	}

	dbg("Saved Tox data of length %d to %s\n", savedata->Data.Data_len,  filename);

	fclose (save);
	return 0;
}
	
int loaddata()
{
	FILE *save;
	struct stat st;
	off_t length;
	char *filename =get_save_filename();
	ToxSaveData_t savedata;

	save =fopen(filename, "rb");
	if (save == NULL) {dbg("Failed to open savefile %s\n", filename); return 1;}

	stat(filename, &st);
	length =st.st_size;
	if (length == 0) 
	{
		fclose(save); dbg ("Save file unusual: length is zero\n"); return 1;
	}

	char data[length];
	fread(data, sizeof(data), 1, save);

	savedata.Data.Data_len =length;
	savedata.Data.Data_val =data;

	toxinstallsavedata_1(savedata, clnt);
	return 0;
}

void getfriendlist()
{
	ToxFriends_t *tst =toxgetfriendlist_1(clnt);
	printf("Friends count: %d\n", tst->Data.Data_len);
	for(int i =0; i < tst->Data.Data_len; i++)
	{
		Contact_t *c =calloc(1, sizeof(Contact_t));
		ToxFriend_t *f =toxgetfriend_1(tst->Data.Data_val[i], clnt);
		c->name =strdup(f->name); c->statusm =strdup(f->statusm); 
		c->pubkey =strdup(f->pubkey); c->num =tst->Data.Data_val[i];
		printf("Friend ID: %d, name %s, status %s, pubkey %s\n", tst->Data.Data_val[i], c->name, c->statusm, c->pubkey);
		List_add(&APP->Xwin->ICQueue, c);
	}
}

int main()
{
	struct rpc_err rpcerr;
	struct timeval tv = { 0 };
	struct timeval newtv = { 0 };

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

	clnt = clnt_create("localhost", TOXAEMIA_PROG, TOXAEMIA_VERS1, "tcp");
	if (clnt == (CLIENT *) NULL) 
	{
		clnt_pcreateerror("localhost");
		exit(1);
	}

	loaddata();
	getfriendlist();

	mtx_init(&APP->Comm->WorkMtx, mtx_plain);
	mtx_init(&APP->Xwin->EventsMtx, mtx_plain);
	thrd_create (&APP->Xwin->Thrd, CXXMain, 0);


	APP->Connected = !toxconnect_1( 
					(int)atoi(Dictionary_get(APP->Config, "Tox.BootstrapPort")), 
					GTC("Tox.BootstrapIP"), GTC("Tox.BootstrapKey"), 
					GTC("Tox.Name"), GTC("Tox.Status") , clnt);

	sleep(1); savedata();

	gettimeofday(&tv, NULL);

	while (!APP->Comm->WantQuit)
	{
		void* work, *tofree =0;
		ToxEvent_t *Event;

		mtx_lock(&APP->Comm->WorkMtx);
		mtx_lock(&APP->Xwin->EventsMtx);

		work =List_retrieve_and_remove_first(&APP->Comm->WorkQueue);
		tofree =work;
		if(!work) goto nowork;

		if(strcmp (work, "savedata") == 0) { savedata(); }
		else if(strncmp (work, "sendfriendrequest", 17) == 0) {sendfriendrequest(work);}
		else if(strcmp (work, "getfriendlist") == 0) {getfriendlist();}
		else dbg("Unhandled request: %s\n", work);
		
		free (tofree);

		nowork:
		gettimeofday(&newtv, NULL);
		if ((newtv.tv_sec - tv.tv_sec) > 1)
		{
			Event =toxgetevent_1(clnt);
			if (Event)	printf("Event %d, param 0 %d, param 1 %s\n", Event->type, Event->param0, Event->param1);
			gettimeofday(&tv, NULL);
		}
		if(!APP->Comm->WorkQueue) APP->Comm->Work =0;
		mtx_unlock(&APP->Comm->WorkMtx);
		mtx_unlock(&APP->Xwin->EventsMtx);
		usleep(5000);
	}
	dbg("Terminating\n");
	Dictionary_write_to_file(APP->Config, APP->ConfigFilename);

	return 0;
}
