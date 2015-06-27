#ifndef _EVCOMM_H_
#define _EVCOMM_H_

#ifdef __cplusplus
extern "C"
{
#endif

#include <threads.h>

#include "toxaemia_rpc.h"

typedef struct Evcomm_s
{
	thrd_t Thread;
	int recv_fd;
	XDR xdr_recv;
} Evcomm_t;

#ifdef __cplusplus
}
#endif

#endif
