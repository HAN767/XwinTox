#include <string.h>
#include <stdlib.h>
#include <unistd.h>

#include "list.h"
#include "misc.h"

#include "toxaemia_rpc.h"
#include "toxaemia_core.h"

List_t *Returns;

int*
toxconnect_1_svc(int BootstrapPort, char* BootstrapAddress, char* BootstrapKey,
				char* Name, char* Status, struct svc_req* SvcReq)
{
	static int result =0;
	if (!Tox_comm) Tox_comm =calloc(1, sizeof(Tox_comm_t));

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

ToxSaveData_t* toxgetsavedata_1_svc()
{
	static ToxSaveData_t result;
	ToxSaveData_t *tmp;
	int *len;
	List_add(&Tox_comm->ICQueue, "getsavedata");

	while (!Returns) usleep (1000);
	tmp =(ToxSaveData_t*)List_retrieve_and_remove_first(&Returns);
	dbg ("Len: %d\n", tmp->Data.Data_len);
	result =*tmp; free (tmp);

	return &result;
}
