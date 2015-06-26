#include <stdlib.h>

#include "postbox.h"

Postbox_t *PB_New()
{
	Postbox_t *newpb =calloc(1, sizeof(Postbox_t));
	newpb->clients =List_new();
	return newpb;
}

void PB_Signal(Postbox_t *pb, PBMTypes mtype, PBMessage_t* msg)
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

void PB_Register(Postbox_t *pb, int mtypes, void* custom,
                 void (*callback)(int, PBMessage_t*, void*))
{
	PBRegistryEntry_t *e =calloc(1, sizeof(PBRegistryEntry_t));
	e->mtypes =mtypes;
	e->callback =callback;
	e->custom =custom;
	List_add(pb->clients, e);
}
