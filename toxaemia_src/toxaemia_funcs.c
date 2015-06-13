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

List_t *Returns;

void ctorTox_comm()
{
	if (!Tox_comm)
	{
	Tox_comm =calloc(1, sizeof(Tox_comm_t));
	mtx_init(&Tox_comm->SaveDataMtx, mtx_plain);
	}
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
	List_add(&Tox_comm->ICQueue, icmsg);

	while (!Tox_comm->SaveData.Data.Data_len) usleep (1000);
	mtx_lock (&Tox_comm->SaveDataMtx);
	result =Tox_comm->SaveData;
	mtx_unlock (&Tox_comm->SaveDataMtx);
	return &result;
}

void* toxinstallsavedata_1_svc(ToxSaveData_t save, struct svc_req* SvcReq)
{
	static int result =0;
	char *data;
	ctorTox_comm();
	dbg("Installing save data\n");
	Tox_comm->SaveData =save;
	data =calloc(save.Data.Data_len, sizeof (unsigned char));
	memcpy(data, save.Data.Data_val, save.Data.Data_len);
	Tox_comm->SaveData.Data.Data_val =data;
	return &result;
}

void* toxsendfriendrequest_1_svc(char *id, char* message, struct svc_req* SvcReq)
{
	static int result =0;
	char *icmsg;

	icmsg =calloc((19 + strlen(id) + 1 + strlen(message)), sizeof(char));
	sprintf(icmsg, "sendfriendrequest %s %s", id, message);
	List_add (&Tox_comm->ICQueue, icmsg);

	return &result;
}

ToxFriends_t* toxgetfriendlist_1_svc()
{
	static ToxFriends_t result;
	ToxFriends_t *returned;
	size_t cnt;
	char *icmsg;

	icmsg =calloc(14, sizeof(char));
	strcpy(icmsg, "getfriendlist");
	List_add(&Tox_comm->ICQueue, icmsg);

	while (!Returns) usleep (1000);
	while (!Returns->Link) usleep (1000);

	result.Data.Data_len =List_retrieve_and_remove_first(&Returns);
	result.Data.Data_val =List_retrieve_and_remove_first(&Returns);

	return &result;
}
