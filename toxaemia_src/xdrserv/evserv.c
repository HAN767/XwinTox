#include <stdlib.h>
#include <unistd.h>

#include "misc.h"

#include "xdrserv.h"
#include "evserv.h"

int Evclient_main(void *custom);

Xdrops_t Evops = { Evclient_main, (void*(*)(void*))Ev_copy, 
					   (void(*)(void*))Ev_free };

int Evclient_main(void *custom)
{
	Xdrclient_t *Xdrclient =custom;

	Xdrclient->xdr_write.x_op =XDR_ENCODE;
	xdrrec_create(&Xdrclient->xdr_write, 0, 0, &Xdrclient->fd,
	              Xdr_readit, Xdr_writeit);

	while(1)
	{
		Event_t *ev;
		mtx_lock(&Xdrclient->Evlock);

		while(!Xdrclient->Evcount)
		{
			cnd_wait(&Xdrclient->Evcond, &Xdrclient->Evlock);
		}

		if(Xdrclient->Events->List) while((ev =List_retrieve_and_remove_first(
			        Xdrclient->Events)) != 0)
			{
				Xdrclient->xdr_write.x_op =XDR_ENCODE;

				if(xdr_Event_t(&Xdrclient->xdr_write, ev) == FALSE)
				{
					dbg("putting: xdr_Event_t failed.\n");
				}

				xdrrec_endofrecord(&Xdrclient->xdr_write, TRUE);
				dbg("Call: %d\n", ev->T, ev->ID);
				Ev_free(ev);
			}

		Xdrclient->Evcount--;
		mtx_unlock(&Xdrclient->Evlock);
	}

	xdr_destroy(&Xdrclient->xdr_write);
	close(Xdrclient->fd);
	return 0;
}

Event_t *Ev_copy(Event_t *ev)
{
	return ev;
}

void Ev_pack(Event_t *ev)
{
	if(!ev->S1) ev->S1 =malloc(4);

	if(!ev->S2) ev->S2 =malloc(4);
}

void Ev_free(Event_t *ev)
{
	free(ev->S1);
	free(ev->S2);
	free(ev);
	return;
}
