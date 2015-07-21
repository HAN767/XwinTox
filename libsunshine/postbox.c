#include <stdlib.h>
#include <threads.h>

#include "misc.h"
#include "list.h"
#include "postbox.h"

typedef struct PB_Thread_Msg_s
{
	int mtype;
	PBMessage_t *msg;
	PB_Callback_f fnCB;
	void *pvCustom;
} PB_Thread_Msg_t;

int PBThread_Main(void *custom)
{
	PBThread_t *self =custom;
	PB_Thread_Msg_t *msg =0;

	while(1)
	{
		mtx_lock(&self->msgMtx);

		while(!self->msgCnt)
		{
			cnd_wait(&self->msgCnd, &self->msgMtx);
		}

		mtx_unlock(&self->msgMtx);

		while((msg =List_retrieve_and_remove_first(
		                 self->msgQueue)) != 0)
		{
			msg->fnCB(msg->mtype, msg->msg, msg->pvCustom);

			mtx_lock(&self->msgMtx);
			self->msgCnt -=1;
			mtx_unlock(&self->msgMtx);
		}
	}
}

Postbox_t *PB_New()
{
	Postbox_t *newpb =calloc(1, sizeof(Postbox_t));
	newpb->clients =List_new();
	newpb->deferred =List_new();
	mtx_init(&newpb->Lock, mtx_plain);

	for (int i =0; i < 2; i++)
	{
		mtx_init(&newpb->threads[i].msgMtx, mtx_plain);
		cnd_init(&newpb->threads[i].msgCnd);
		newpb->threads[i].msgCnt =0;
		newpb->threads[i].msgQueue =List_new();
		thrd_create(&newpb->threads[i].thrd, PBThread_Main, &newpb->threads[i]);
	}

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

void PB_Signal_Multithreaded(Postbox_t *pb, int mtype, PBMessage_t *msg)
{
	LIST_ITERATE_OPEN(pb->clients)
	if(((PBRegistryEntry_t*) e_data)->mtype == mtype)
	{
		PB_Thread_Msg_t *pbtMsg =malloc(sizeof(PB_Thread_Msg_t));
		pbtMsg->mtype =mtype;
		pbtMsg->msg =msg;
		pbtMsg->fnCB =((PBRegistryEntry_t*) e_data)->callback;
		pbtMsg->pvCustom =((PBRegistryEntry_t*) e_data)->custom;

		mtx_lock(&pb->threads[0].msgMtx);
		List_add(pb->threads[0].msgQueue, pbtMsg);
		pb->threads[0].msgCnt +=1;
		cnd_broadcast(&pb->threads[0].msgCnd);
		mtx_unlock(&pb->threads[0].msgMtx);
	}
	LIST_ITERATE_CLOSE(pb->clients)
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
