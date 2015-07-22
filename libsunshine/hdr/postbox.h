#ifndef _POSTBOX_H_
#define _POSTBOX_H_

#ifdef __cplusplus
extern "C"
{
#endif

#include <threads.h>

#include "sptr.h"
#include "list.h"

typedef enum PBPriority_e
{
	PB_Slow,
	PB_Fast,
} PBPriority_e;


/* Ownership policy for PBMessages:
 * Strings are owned by the Postbox. They are copied.
 * Void pointers are owned by the recipient;
 * they are not copied. */

typedef struct PBMessage_s
{
	const char *ORIGIN;
	PBPriority_e P;
	int filtered; /* used to indicate it has been filtered already */ 
	void *V; /* not copied */
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

typedef struct Postbox_s Postbox_t;

typedef struct PBThread_s
{
	thrd_t thrd;
	List_t *msgQueue;
	short msgCnt;
	mtx_t msgMtx;
	cnd_t msgCnd;
	Postbox_t *postbox;
} PBThread_t;

typedef struct Postbox_s
{
	List_t *clients, *deferred, *filters;
	mtx_t Lock;
	PBThread_t threads[3]; /* 0 = slow, 1 = fast */
} Postbox_t;

Postbox_t *PB_New();
void PB_Defer(Postbox_t *pb, int mtype, PBMessage_t *msg);
void PB_Signal(Postbox_t *pb, int mtype, PBMessage_t* msg);
void PB_Signal_Multithreaded(Postbox_t *pb, int mtype, PBMessage_t* msg);
void PB_Despatch_Deferred(Postbox_t *pb);
void PB_Register(Postbox_t *pb, int mtype, void *custom, PB_Callback_f);
void PB_Register_Filter(Postbox_t *pb, int mtype, void *custom, PB_Callback_f);

static inline PBMessage_t *PB_New_Message()
{
	PBMessage_t *newPBM =(PBMessage_t*) calloc(1, sizeof(PBMessage_t));
	newPBM->filtered =0;
	newPBM->P =PB_Fast;
	return newPBM;
}

static inline void PB_Free_Message(PBMessage_t *pmsgMsg)
{
	if(pmsgMsg->S1) free(pmsgMsg->S1);
	if(pmsgMsg->S2) free(pmsgMsg->S2);
	if(pmsgMsg->S3) free(pmsgMsg->S3);
	if(pmsgMsg->S4) free(pmsgMsg->S4);
	pmsgMsg->V =0;
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
