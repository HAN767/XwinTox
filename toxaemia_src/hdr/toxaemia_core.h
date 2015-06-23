#ifndef TOXAEMIA_CORE_H
#define TOXAEMIA_CORE_H

#include <threads.h>

#include <tox/tox.h>

#include "list.h"

#include "toxaemia_rpc.h"

typedef struct Tox_comm_s
{
	thrd_t Thrd;
	List_t* ICQueue;

	int Connected;
	cnd_t ConnectedCnd;
	mtx_t ConnectedMtx;

	Tox* tox;

	int BootstrapPort;
	char* BootstrapAddress;
	char* BootstrapKey;
	char* Name;
	char* Status;

	ToxSaveData_t SaveData;
	mtx_t SaveDataMtx;
} Tox_comm_t;

typedef struct Call_s
{
	unsigned long Func;
	char *S1, *S2;
	int I1, I2;
} Call_t;

extern Tox_comm_t *Tox_comm;
extern int Tox_thread_launched;
extern List_t *Calls;
extern List_t *Returns;
extern List_t *Events;

int launch_tox_thread();

#endif
