#include <stdlib.h>
#include <threads.h>

#ifdef __cplusplus
#include <FL/FL.H>
#endif

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

	if(((PBRegistryEntry_t*) e_data)->mtypes & mtype)
	{
		((PBRegistryEntry_t*) e_data)->
		callback(mtype, msg,
		         ((PBRegistryEntry_t*) e_data)->custom);
	}

	LIST_ITERATE_CLOSE(pb->clients)

	if(msg->S1) free(msg->S1);

	if(msg->S2) free(msg->S2);

	free(msg);
}

/* note: implement a second PB_Signal that launches a seperate thread 
 * for despatch later */

void PB_Despatch_Deferred(Postbox_t *pb)
{
	PBDeferred_t *d =0;

	while((d =List_retrieve_and_remove_first(pb->deferred)) != 0)
	{
		LIST_ITERATE_OPEN(pb->clients)

		if(((PBRegistryEntry_t*) e_data)->mtypes & d->mtype)
		{
			((PBRegistryEntry_t*) e_data)->
			callback(d->mtype, d->msg,
			         ((PBRegistryEntry_t*) e_data)->custom);
		}

		LIST_ITERATE_CLOSE(pb->clients)

		if(d->msg->S1) free(d->msg->S1);

		if(d->msg->S2) free(d->msg->S2);

		free(d->msg);
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

void PB_Register(Postbox_t *pb, int mtypes, void* custom,
                 void (*callback)(int, PBMessage_t*, void*))
{
	PBRegistryEntry_t *e =calloc(1, sizeof(PBRegistryEntry_t));
	e->mtypes =mtypes;
	e->callback =callback;
	e->custom =custom;
	List_add(pb->clients, e);
}
