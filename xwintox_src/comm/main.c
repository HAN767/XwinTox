#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/time.h>
#include <threads.h>
#include <unistd.h>

#include "misc.h"
#include "list.h"
#include "dictionary.h"

#include "toxaemia_rpc.h"

#include "xwintox.h"
#include "etc.h"
#include "postbox.h"

#define GTC(X) (char *)Dictionary_get (APP->Config, X )

XwinTox_instance_t *APP;
CLIENT *clnt;

extern int CXXMain();
extern int Resolv_main();

int sendfriendrequest(char* Rmsg)
{
	char *id;
	int *result;

	strsep(&Rmsg, " ");
	id =Rmsg;
	strsep(&Rmsg, " ");

	/* try to add tox ID */
	if(strlen(id) == 72)
	{
		dbg("Adding regular Tox ID\n");
	}

	if((result =toxsendfriendrequest_1(id, Rmsg, clnt)) == 0)
	{
		clnt_perror(clnt, "Sendfriendrequest");
		return 1;
	}
	else
	{
		ToxEvent_t *fadd =calloc(1, sizeof(ToxEvent_t));
		fadd->type =FADDED;
		fadd->paramid =*result;
		fadd->param0 =1; /* 1 indicates internally generated, friend added
							after successful request */
		fadd->param1 =calloc(1, sizeof(char));
		fadd->param2.param2_len =0;
		List_add(APP->Xwin->Events, fadd);
	}

	return 0;
}

int addfriendnorequest(char* Rmsg)
{
	char *id;
	int *result;

	id =Rmsg;

	if((result =toxaddfriendnorequest_1(id, clnt)) == 0)
	{
		clnt_perror(clnt, __func__);
		return 1;
	}
	else
	{
		ToxEvent_t *fadd =calloc(1, sizeof(ToxEvent_t));
		fadd->type =FADDED;
		fadd->paramid =*result;
		fadd->param0 =1; /* 1 indicates internally generated, friend added
							after successful request */
		fadd->param1 =calloc(1, sizeof(char));
		fadd->param2.param2_len =0;
		List_add(APP->Xwin->Events, fadd);
	}

	return 0;
}

int savedata()
{
	FILE *save;
	char *filename =get_save_filename();
	ToxSaveData_t *savedata;

	savedata =toxgetsavedata_1(clnt);

	if(!savedata)
	{
		clnt_perror(clnt, "Savedata");
		return 1;
	}

	unlink(filename);
	save =fopen(filename, "wb");

	if(save == NULL)
	{
		dbg("Failed to open savefile %s\n", filename);
		return 1;
	}

	if(fwrite(savedata->Data.Data_val, savedata->Data.Data_len-1, 1, save) !=1)
	{
		dbg("Failed to save data to savefile %s\n", filename);
		fclose(save);
		return 1;
	}

	dbg("Saved Tox data of length %d to %s\n", savedata->Data.Data_len,
	    filename);

	fclose(save);

	Dictionary_write_to_file(APP->Config, APP->ConfigFilename);

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

	if(save == NULL)
	{
		dbg("Failed to open savefile %s\n", filename);
		return 1;
	}

	stat(filename, &st);
	length =st.st_size;

	if(length == 0)
	{
		fclose(save);
		dbg("Save file unusual: length is zero\n");
		return 1;
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

	if(tst->Data.Data_len <1) return;

	if(tst->Data.Data_len == 1)
	{
		Contact_t *c =calloc(1, sizeof(Contact_t));
		ToxFriend_t *f =toxgetfriend_1(tst->Data.Data_val[0], clnt);
		c->status =f->status;
		c->connected =f->connected;
		c->name =strdup(f->name);
		c->statusm =strdup(f->statusm);
		c->pubkey =strdup(f->pubkey);
		c->num =tst->Data.Data_val[0];
		printf("Friend ID: %d, name %s, status %s, pubkey %s\n",
		       tst->Data.Data_val[0], c->name, c->statusm, c->pubkey);
		List_add(APP->Xwin->ICQueue, c);
		dbg("Alternative pathway completed\n");
		return;
	}

	for(int i =0; i < tst->Data.Data_len; i++)
	{
		Contact_t *c =calloc(1, sizeof(Contact_t));
		ToxFriend_t *f =toxgetfriend_1(tst->Data.Data_val[i], clnt);
		c->status =f->status;
		c->connected =f->connected;
		c->name =strdup(f->name);
		c->statusm =strdup(f->statusm);
		c->pubkey =strdup(f->pubkey);
		c->num =tst->Data.Data_val[i];
		printf("Friend ID: %d, name %s, status %s, pubkey %s\n",
		       tst->Data.Data_val[i], c->name, c->statusm, c->pubkey);
		List_add(APP->Xwin->ICQueue, c);
		dbg("Primary pathway completed\n");
	}
}

int sendmessage(char* Rmsg)
{
	char *type, *id;

	strsep(&Rmsg, " ");
	type =Rmsg;
	strsep(&Rmsg, " ");
	id =Rmsg;
	strsep(&Rmsg, " ");

	if(!toxsendmessage_1(strtol(type, 0, 10), strtol(id, 0, 10), Rmsg, clnt))
	{
		clnt_perror(clnt, "Sendmessage");
		return 1;
	}

	return 0;
}

int deletefriend(char* Rmsg)
{
	strsep(&Rmsg, " ");

	if(!toxdeletefriend_1(strtol(Rmsg, 0, 10), clnt))
	{
		clnt_perror(clnt, "Deletecontact");
		return 1;
	}

	savedata();
	return 0;
}

void newgroupchat()
{
	int* res =toxcreategroupchat_1(clnt);

	if(!res)
	{
		clnt_perror(clnt, "Newgroupchat");
		return;
	}
	else
	{
		ToxEvent_t *fadd =calloc(1, sizeof(ToxEvent_t));
		fadd->type =GNEW;
		fadd->paramid =*res;
		fadd->param0 =1; /* 1 indicates internally generated, group added
							after successful request */
		fadd->param1 =calloc(1, sizeof(char));
		fadd->param2.param2_len =0;
		List_add(APP->Xwin->Events, fadd);
	}

	dbg("Groupchat: %d\n", *res);
}

void namechange()
{

	int* res =toxsetnamestatus_1(GTC("Tox.Name"), GTC("Tox.Status"), clnt);

	if(!res)
	{
		clnt_perror(clnt, __func__);
		return;
	}

	savedata();
}

void leavegroupchat(char *Rmsg)
{
	strsep(&Rmsg, " ");

	if(!toxleavegroupchat_1(strtol(Rmsg, 0, 10), clnt))
	{
		clnt_perror(clnt, __func__);
		return;
	}
}

int main()
{
	struct timeval tv = { 0 };
	struct timeval newtv = { 0 };

	/* component independent main */
	APP =calloc(1, sizeof(XwinTox_instance_t));
	APP->Comm =calloc(1, sizeof(Comm_t));
	APP->Xwin =calloc(1, sizeof(Xwin_t));
	APP->Resolv =calloc(1, sizeof(Resolv_t));
	APP->Evcomm =calloc(1, sizeof(Xdrcomm_t));
	APP->Filecomm =calloc(1, sizeof(Xdrcomm_t));

	APP->Comm->WorkQueue =List_new();
	APP->Xwin->Events =List_new();
	APP->Xwin->ICQueue =List_new();
	APP->Resolv->Calls =List_new();
	APP->Evcomm->port ="5554";
	APP->Filecomm->port ="5556";

	APP->Config =Dictionary_new(24);
	APP->ConfigFilename =get_config_filename();
	APP->events =PB_New();

	umask(S_IRGRP | S_IWGRP | S_IROTH | S_IWOTH);

	create_config_folder();

	Dictionary_load_from_file(APP->Config, APP->ConfigFilename, 1);
	default_config(APP->Config);

	clnt = clnt_create("localhost", TOXAEMIA_PROG, TOXAEMIA_VERS1, "tcp");

	if(clnt == (CLIENT *) NULL)
	{
		clnt_pcreateerror("localhost");
		exit(1);
	}

	loaddata();
	getfriendlist();

	mtx_init(&APP->Comm->WorkMtx, mtx_plain);
	mtx_init(&APP->Xwin->EventsMtx, mtx_plain);
	mtx_init(&APP->Resolv->CallsMtx, mtx_plain);
	cnd_init(&APP->Resolv->CallsCnd);

	thrd_create(&APP->Evcomm->Thread, Xdrcomm_main, APP->Evcomm);
	thrd_create(&APP->Xwin->Thrd, CXXMain, 0);
	thrd_create(&APP->Resolv->Thread, Resolv_main, 0);

	APP->Connected = !toxconnect_1(
	                     (int)atoi(Dictionary_get(APP->Config, "Tox.BootstrapPort")),
	                     GTC("Tox.BootstrapIP"), GTC("Tox.BootstrapKey"),
	                     GTC("Tox.Name"), GTC("Tox.Status") , clnt);

	sleep(1);
	savedata();

	gettimeofday(&tv, NULL);

	while(!APP->Comm->WantQuit)
	{
		void* work, *tofree =0;
		ToxEvent_t *Event;

		mtx_lock(&APP->Comm->WorkMtx);
		mtx_lock(&APP->Xwin->EventsMtx);

		work =List_retrieve_and_remove_first(APP->Comm->WorkQueue);
		tofree =work;

		if(!work) goto nowork;

		if(strcmp(work, "savedata") == 0)
		{
			savedata();
		}
		else if(strncmp(work, "sendfriendrequest", 17) == 0)
		{
			sendfriendrequest(work);
		}
		else if(strncmp(work, "addfriendnoreq", 14) == 0)
		{
			strsep((char**)&work, " ");
			addfriendnorequest(work);
		}
		else if(strcmp(work, "getfriendlist") == 0)
		{
			getfriendlist();
		}
		else if(strncmp(work, "sendmessage", 11) == 0)
		{
			sendmessage(work);
		}
		else if(strncmp(work, "deletecontact", 13) == 0)
		{
			deletefriend(work);
		}
		else if(strcmp(work, "newgroupchat") == 0)
		{
			newgroupchat();
		}
		else if(strcmp(work, "namechange") == 0)
		{
			namechange();
		}
		else if (strncmp(work, "leavegroupchat", 14) == 0)
		{
			leavegroupchat(work);
		}
		else dbg("Unhandled request: %s\n", work);

		free(tofree);

nowork:
		gettimeofday(&newtv, NULL);

		if((newtv.tv_sec - tv.tv_sec) > 1)
		{
			while(((Event =toxgetevent_1(clnt)) != 0) && Event &&
			        Event->type != 0)
			{
				ToxEvent_t *NEvent =calloc(1, sizeof(ToxEvent_t));
				*NEvent =*Event;
				NEvent->param1 =strdup(Event->param1);

				NEvent->param2.param2_val =calloc(Event->param2.param2_len,
				                                  sizeof(char));
				memcpy(NEvent->param2.param2_val, Event->param2.param2_val,
				       Event->param2.param2_len);
				NEvent->param2.param2_len =Event->param2.param2_len;

				NEvent->param3.param3_val =calloc(Event->param3.param3_len,
				                                  sizeof(short));
				memcpy(NEvent->param3.param3_val, Event->param3.param3_val,
				       Event->param3.param3_len * sizeof(short));
				NEvent->param3.param3_len =Event->param3.param3_len;

				List_add(APP->Xwin->Events, NEvent);
			}

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
