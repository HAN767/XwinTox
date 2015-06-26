#ifndef _SIGNAL_H
#define _SIGNAL_H

#ifdef __cplusplus
extern "C"
{
#endif

#include "list.h"


typedef enum PBMTypes_e
{
	PB_FRequest =1,
	PB_FReqServiced =2
}
PBMTypes;

typedef struct PBMessage_s
{
	char *S1, *S2, *S3;
	int *I1, *I2, *I3;
} PBMessage_t;

typedef struct PBRegistryEntry_s
{
	int mtypes;
	void (*callback)(int, PBMessage_t*, void*);
	void *custom;
} PBRegistryEntry_t;

typedef struct Postbox_s
{
	List_t *clients;
} Postbox_t;

Postbox_t *PB_New();
void PB_Signal(Postbox_t *pb, PBMTypes mtype, PBMessage_t* msg);
void PB_Register(Postbox_t *pb, int mtypes, void*,
                 void (*callback)(int, PBMessage_t*, void*));

#ifdef __cplusplus
}
#endif

#endif
