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
	List_t *Messages;
	XDR xdr_write;
	int fd;
} Evclient_t;

typedef struct Evserv_s
{
	thrd_t Thread;
	List_t *Clients;
	List_t *Events;
	int fd;
} Evserv_t;

int Evserv_main(void *custom);


#endif
