#include <stdlib.h>
#include <unistd.h>

#include "misc.h"

#include "xdrserv.h"
#include "evserv.h"
#include "fileserv.h"

int Fileclient_main(void *custom);

Xdrops_t Fileops = { Fileclient_main, (void*(*)(void*))Ev_copy, 
					   (void(*)(void*))Ev_free };

int Fileclient_main(void *custom)
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
