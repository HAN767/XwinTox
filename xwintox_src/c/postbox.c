#include <stdlib.h>

#include "signal.h"

void PB_Signal(Postbox_t *pb, PBMTypes mtype, PBMessage_t* msg)
{
	LIST_ITERATE(pb->clients)
	{
		if(((PBRegistryEntry_t*) e_data)->mtypes & mtype)
		{
			((PBRegistryEntry_t*) e_data)->
			callback(mtype, msg,
			         ((PBRegistryEntry_t*) e_data)->custom);
		}
	}

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
	List_add(&pb->clients, e);
}
