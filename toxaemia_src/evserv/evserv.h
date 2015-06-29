#ifndef __EVSERV_H__
#define __EVSERV_H__

#include <threads.h>

#include "list.h"
#include "toxaemia_rpc.h"

#define PORT 5554
#define BUFFER_SIZE 524288

typedef struct Evclient_s
{
	thrd_t Thread;
	List_t *Events;
	int Evcount;
	mtx_t Evlock;
	cnd_t Evcond;
	XDR xdr_write;
	int fd;
} Evclient_t;

typedef struct Evserv_s
{
	thrd_t Thread;
	List_t *Clients;
	int fd;
} Evserv_t;

int Evserv_main(void *custom);
void Ev_pack(Event_t *ev);
void Ev_free(Event_t *ev);
Event_t *Ev_copy(Event_t *ev);
void Evserv_send_event(Evserv_t *evserv, Event_t *ev);



#endif
