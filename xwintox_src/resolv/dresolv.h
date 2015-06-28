#ifndef _DRESOLV_H_
#define _DRESOLV_H_

#include <stdint.h>
#include <threads.h>

typedef enum ResolvCalls_e
{
	R_DNSResolve
} ResolvCalls;

typedef struct Tox3_s
{
	const char *name;
	uint8_t key[32];
} Tox3_t;

#define ResolvAddWork(CNT) \
	mtx_lock(&APP->Resolv->CallsMtx); \
	APP->Resolv->CallsCnt += CNT ; \
	cnd_broadcast(&APP->Resolv->CallsCnd); \
	mtx_unlock(&APP->Resolv->CallsMtx)

#endif
