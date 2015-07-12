#include <stdlib.h>
#include <threads.h>

#ifdef __cplusplus
#include <FL/FL.H>
#endif

#include "list.h"
#include "postbox.h"

Postbox_t *PB_New()
{
	Postbox_t *newpb =calloc(1, sizeof(Postbox_t));
	newpb->clients =List_new();
	newpb->deferred =List_new();
	mtx_init(&newpb->Lock, mtx_plain);
	return newpb;
}

void PB_Signal(Postbox_t *pb, int mtype, PBMessage_t *msg) 
{
	LIST_ITERATE_OPEN(pb->clients)

	if(((PBRegistryEntry_t*) e_data)->mtype == mtype)
	{
		((PBRegistryEntry_t*) e_data)->
		callback(mtype, msg,
		         ((PBRegistryEntry_t*) e_data)->custom);
	}

	LIST_ITERATE_CLOSE(pb->clients)

	PB_Free_Message(msg);
}

typedef struct PB_Thread_Msg_s
{
	thrd_t thrd;
	int mtype;
	Shared_Ptr_t *sprMsg;
	PB_Callback_f fnCB;
	void *pvCustom;
} PB_Thread_Msg_t;

int PB_Despatch_Deferred_Thread(void *pvCustom)
{
	PB_Thread_Msg_t *tMsg =(PB_Thread_Msg_t*)pvCustom;

	tMsg->fnCB(tMsg->mtype, tMsg->sprMsg->pvData, tMsg->pvCustom);

	PBM_DEC(tMsg->sprMsg);
	free(tMsg);
	thrd_exit(0);
}

void PB_Signal_Multithreaded(Postbox_t *pb, int mtype, PBMessage_t *msg)
{
	List_t *lstHandlers;
	Shared_Ptr_t *sprMsg =Shared_Ptr_new(msg);

	LIST_ITERATE_OPEN(pb->clients)
	if(((PBRegistryEntry_t*) e_data)->mtype == mtype)
	{
		PB_Thread_Msg_t *pbtMsg =malloc(sizeof(PB_Thread_Msg_t));
		pbtMsg->mtype =mtype;
		pbtMsg->sprMsg =sprMsg;
		pbtMsg->fnCB =((PBRegistryEntry_t*) e_data)->callback;
		pbtMsg->pvCustom =((PBRegistryEntry_t*) e_data)->custom;
		if (thrd_create(&pbtMsg->thrd, PB_Despatch_Deferred_Thread,
								  pbtMsg)) PBM_INC(sprMsg);
	}
	LIST_ITERATE_CLOSE(pb->clients)

	PBM_DEC(sprMsg);
}

void PB_Despatch_Deferred(Postbox_t *pb)
{
	PBDeferred_t *d =0;

	while((d =List_retrieve_and_remove_first(pb->deferred)) != 0)
	{
		LIST_ITERATE_OPEN(pb->clients)

		if(((PBRegistryEntry_t*) e_data)->mtype == d->mtype)
		{
			((PBRegistryEntry_t*) e_data)->
			callback(d->mtype, d->msg,
			         ((PBRegistryEntry_t*) e_data)->custom);
		}

		LIST_ITERATE_CLOSE(pb->clients)

		PB_Free_Message(d->msg);
		free(d);
	}
}

void PB_Defer(Postbox_t *pb, int mtype, PBMessage_t *msg)
{
	PBDeferred_t *newdf =calloc(1, sizeof(PBDeferred_t));

	newdf->msg =msg;
	newdf->mtype =mtype;

	List_add(pb->deferred, newdf);
#ifdef __cplusplus
	Fl::awake();
#endif
}

void PB_Register(Postbox_t *pb, int mtype, void* custom,
                 void (*callback)(int, PBMessage_t*, void*))
{
	PBRegistryEntry_t *e =calloc(1, sizeof(PBRegistryEntry_t));
	e->mtype =mtype;
	e->callback =callback;
	e->custom =custom;
	List_add(pb->clients, e);
}
