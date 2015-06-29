/* Note:
 * As the lists implementation is thread-safe, any apparently unusual waiting
 * on the value of list pointers is not harmful, and will behave correctly.
 */

#include <string.h>
#include <stdio.h> // sprintf()
#include <stdlib.h>
#include <unistd.h>

#include "list.h"
#include "misc.h"

#include "toxaemia_rpc.h"
#include "toxaemia_core.h"
#include "evserv/evserv.h"

Evserv_t *Evserv =0;

void ctorEvserv()
{
	if(!Evserv)
	{
		Evserv =calloc(1, sizeof(Evserv_t));
		Evserv->Events =List_new();
		Evserv->Clients =List_new();
		thrd_create(&Evserv->Thread, Evserv_main, Evserv);
	}
}

void ctorTox_comm_onlycreate()
{
	ctorEvserv();
	if(!Tox_comm)
	{
		Tox_comm =calloc(1, sizeof(Tox_comm_t));
		mtx_init(&Tox_comm->SaveDataMtx, mtx_plain);
	}
}

void ctorTox_comm()
{
	ctorTox_comm_onlycreate();

	Tox_comm->ICQueue =List_new();

	if(!Tox_thread_launched) launch_tox_thread();
}

int*
toxconnect_1_svc(int BootstrapPort, char* BootstrapAddress, char* BootstrapKey,
                 char* Name, char* Status, struct svc_req* SvcReq)
{
	static int result =0;
	ctorTox_comm();

	Tox_comm->BootstrapPort =BootstrapPort;
	Tox_comm->BootstrapAddress =strdup(BootstrapAddress);
	Tox_comm->BootstrapKey =strdup(BootstrapKey);
	Tox_comm->Name =strdup(Name);
	Tox_comm->Status =strdup(Status);

	if(!Tox_thread_launched) launch_tox_thread();

	char *icmsg =calloc(8, sizeof(char));
	strcpy(icmsg, "connect");
	List_add(Tox_comm->ICQueue, icmsg);


	return &result;
}

void* toxdisconnect_1_svc(struct svc_req* SvcReq)
{
	static int result =0;
	return &result;
}

ToxSaveData_t* toxgetsavedata_1_svc(struct svc_req* SvcReq)
{
	static ToxSaveData_t result;
	char* icmsg = calloc(12, sizeof(char));

	Tox_comm->SaveData.Data.Data_len =0;

	if(Tox_comm->SaveData.Data.Data_val) free(Tox_comm->SaveData.Data.Data_val);

	strcpy(icmsg, "getsavedata");
	List_add(Tox_comm->ICQueue, icmsg);

	while(!Tox_comm->SaveData.Data.Data_len) usleep(1000);

	mtx_lock(&Tox_comm->SaveDataMtx);
	result =Tox_comm->SaveData;
	mtx_unlock(&Tox_comm->SaveDataMtx);
	return &result;
}

void* toxinstallsavedata_1_svc(ToxSaveData_t save, struct svc_req* SvcReq)
{
	static int result =0;
	char *data;
	ctorTox_comm_onlycreate();
	dbg("Installing save data\n");
	Tox_comm->SaveData =save;
	data =calloc(save.Data.Data_len, sizeof(char));
	memcpy(data, save.Data.Data_val, save.Data.Data_len);
	Tox_comm->SaveData.Data.Data_val =data;
	return &result;
}

void* toxsetnamestatus_1_svc(char* name, char* status, struct svc_req* SvcReq)
{
	static int result =0;
	Call_t *call =calloc(1, sizeof(Call_t));

	call->Func =ToxSetNameStatus;
	call->S1 =strdup(name);
	call->S2 =strdup(status);
	List_add(Calls, call);

	return &result;
}

int* toxsendfriendrequest_1_svc(char *id, char* message, struct svc_req* SvcReq)
{
	static int result =-1;
	char *icmsg;

	icmsg =calloc((19 + strlen(id) + 1 + strlen(message)), sizeof(char));
	sprintf(icmsg, "sendfriendrequest %s %s", id, message);
	List_add(Tox_comm->ICQueue, icmsg);

	while(!Returns->List) usleep(1000);

	result =(int)List_retrieve_and_remove_first(Returns);

	return &result;
}

ToxFriends_t* toxgetfriendlist_1_svc(struct svc_req *SvcReq)
{
	static ToxFriends_t result;
	char *icmsg;

	ctorTox_comm();

	icmsg =calloc(14, sizeof(char));
	strcpy(icmsg, "getfriendlist");
	List_add(Tox_comm->ICQueue, icmsg);

	while(!Returns->List) usleep(1000);

	while(!Returns->List->Link) usleep(1000);

	result.Data.Data_len =(unsigned int)List_retrieve_and_remove_first(Returns);
	result.Data.Data_val =List_retrieve_and_remove_first(Returns);

	return &result;
}

ToxFriend_t* toxgetfriend_1_svc(unsigned int num, struct svc_req* SvcReq)
{
	static ToxFriend_t result;
	char *icmsg;
	ctorTox_comm();

	icmsg =calloc(10 + 10, sizeof(char));
	sprintf(icmsg, "getfriend %d", num);
	List_add(Tox_comm->ICQueue, icmsg);

	while(!Returns->List) usleep(1000);

	while(!Returns->List->Link) usleep(1000);

	while(!Returns->List->Link->Link) usleep(1000);

	while(!Returns->List->Link->Link->Link) usleep(1000);

	while(!Returns->List->Link->Link->Link->Link) usleep(1000);

	result.name =List_retrieve_and_remove_first(Returns);
	result.statusm =List_retrieve_and_remove_first(Returns);
	result.pubkey =List_retrieve_and_remove_first(Returns);
	result.status =(int)List_retrieve_and_remove_first(Returns);
	result.connected =(short)List_retrieve_and_remove_first(Returns);

	return &result;
}

ToxEvent_t* toxgetevent_1_svc(struct svc_req* SvcReq)
{
	static ToxEvent_t result;
	ToxEvent_t *ptr, *ret;

	result.type =0;
	result.paramid =0;
	result.param0 =0;
	result.param1 =" ";
	result.param2.param2_len =0;

	ptr =List_retrieve_and_remove_first(Events);

	if(ptr != 0) ret =ptr;
	else ret =&result;

	return ret;
}

void* toxsendmessage_1_svc(ToxMessageType type, unsigned int id, char* message,
                           struct svc_req* SvcReq)
{
	char *icmsg;

	dbg("Send message to id %d, message %s\n", id, message);
	icmsg =calloc(17 + strlen(message), sizeof(char));
	sprintf(icmsg, "sendmessage %d %d %s", type, id, message);
	List_add(Tox_comm->ICQueue, icmsg);

	return (void*)1;
}

void* toxdeletefriend_1_svc(unsigned int num, struct svc_req* SvcReq)
{
	static int ret =1;

	tox_friend_delete(Tox_comm->tox, num, 0);

	return &ret;
}

int* toxcreategroupchat_1_svc(struct svc_req *SvcReq)
{
	static int ret =-1;
	char *icmsg;

	icmsg =strdup("creategroupchat");
	List_add(Tox_comm->ICQueue, icmsg);

	while(!Returns->List) usleep(1000);

	ret =(int)List_retrieve_and_remove_first(Returns);

	return &ret;
}

void* toxleavegroupchat_1_svc(unsigned int num, struct svc_req *SvcReq)
{
	static int result =0;
	Call_t *call =calloc(1, sizeof(Call_t));

	call->Func =ToxLeaveGroupchat;
	call->I1 =num;
	List_add(Calls, call);

	return &result;
}

int *toxaddfriendnorequest_1_svc(char* address, struct svc_req *SvcReq)
{
	static int ret =-1;
	Call_t *call =calloc(1, sizeof(Call_t));

	call->Func =ToxAddFriendNoRequest;
	call->S1 =strdup(address);
	List_add(Calls, call);

	while(!Returns->List) usleep(1000);

	ret =(int)List_retrieve_and_remove_first(Returns);
	dbg("result: %d\n", ret);

	return &ret;
}
