#include <string.h>
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
	List_add(&Tox_comm->ICQueue, "getsavedata");

	while (!Tox_comm->SaveData.Data.Data_len) usleep (1000);
	mtx_lock (&Tox_comm->SaveDataMtx);
	result =Tox_comm->SaveData;
	mtx_unlock (&Tox_comm->SaveDataMtx);
	return &result;
}

void* toxinstallsavedata_1_svc(ToxSaveData_t save, struct svc_req* SvcReq)
{
	static int result =0;
	unsigned char *data;
	ctorTox_comm();
	dbg("Installing save data\n");
	Tox_comm->SaveData =save;
	data =calloc(save.Data.Data_len, sizeof (unsigned char));
	memcpy(data, save.Data.Data_val, save.Data.Data_len);
	Tox_comm->SaveData.Data.Data_val =data;
	return &result;
}
