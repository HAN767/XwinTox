#ifndef _POSTBOX_H_
#define _POSTBOX_H_

#ifdef __cplusplus
extern "C"
{
#endif

#include <threads.h>

#include "sptr.h"
#include "list.h"

typedef struct PBMessage_s
{
	const char *ORIGIN;
	char *S1, *S2, *S3, *S4;
	int I1, I2, I3, I4;
} PBMessage_t;

typedef void (*PB_Callback_f)(int, PBMessage_t*, void*);

typedef struct PBRegistryEntry_s
{
	int mtype;
	PB_Callback_f callback;
	void *custom;
} PBRegistryEntry_t;

typedef struct PBDeferred_s
{
	int mtype;
	PBMessage_t *msg;
} PBDeferred_t;

typedef struct Postbox_s
{
	List_t *clients;
	List_t *deferred;
	mtx_t Lock;
} Postbox_t;

typedef struct PBC_s
{
	int mtype;
	PBMessage_t *msg;
} PBC_t; /* provided for convenience */

Postbox_t *PB_New();
void PB_Defer(Postbox_t *pb, int mtype, PBMessage_t *msg);
void PB_Signal(Postbox_t *pb, int mtype, PBMessage_t* msg);
void PB_Signal_Multithreaded(Postbox_t *pb, int mtype, PBMessage_t *msg);
void PB_Despatch_Deferred(Postbox_t *pb);
void PB_Register(Postbox_t *pb, int mtype, void *custom, PB_Callback_f);

static inline void PB_Free_Message(PBMessage_t *pmsgMsg)
{
		if(pmsgMsg->S1) free(pmsgMsg->S1);
		if(pmsgMsg->S2) free(pmsgMsg->S2);
		if(pmsgMsg->S3) free(pmsgMsg->S3);
		if(pmsgMsg->S4) free(pmsgMsg->S4);
		free(pmsgMsg);
}

/* This section is for refcnting. Postbox private. */
#define PBM_INC(X) Shared_Ptr_inc(X)
static inline void PBM_DEC(Shared_Ptr_t *psprSptr)
{
	mtx_lock(&psprSptr->mtxCnt);
	psprSptr->wCnt -=1;
	if(psprSptr->wCnt == 0)
	{
		PB_Free_Message((PBMessage_t*)psprSptr->pvData);
		free(psprSptr);
	}
	else
	{
		mtx_unlock(&psprSptr->mtxCnt);
	}
	return;
}


#ifdef __cplusplus
}
#endif

#endif
