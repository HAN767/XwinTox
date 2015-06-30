#ifndef _EVCOMM_H_
#define _EVCOMM_H_

#ifdef __cplusplus
extern "C"
{
#endif

#include <threads.h>

#include "toxaemia_rpc.h"

typedef struct Xdrcomm_s
{
	thrd_t Thread;
	char *port;
	int recv_fd;
	XDR xdr_recv;
} Xdrcomm_t;

int Xdrcomm_main(void *data);

#ifdef __cplusplus
}
#endif

#endif
