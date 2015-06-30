#ifndef __EVSERV_H__
#define __EVSERV_H__

#include <threads.h>

#include "list.h"
#include "toxaemia_rpc.h"

typedef struct Xdrserv_s Xdrserv_t;

typedef struct Xdrops_s
{
	thrd_start_t clmain;
	void *(*copy)(void*);
	void (*free)(void*);
} Xdrops_t;

typedef struct Xdrclient_s
{
	thrd_t Thread;
	List_t *Events;
	int Evcount;
	mtx_t Evlock;
	cnd_t Evcond;
	XDR xdr_write;
	int fd;
	Xdrops_t *ops;
} Xdrclient_t;

typedef struct Xdrserv_s
{
	thrd_t Thread;
	List_t *Clients;
	int fd, port;
	Xdrops_t *ops;
} Xdrserv_t;

int Xdrserv_main(void *custom);
int Xdr_readit(void* handle, void* buf, int len);
int Xdr_writeit(void* handle, void* buf, int len);
void Xdrserv_send(Xdrserv_t *evserv, void *ev);


#endif
