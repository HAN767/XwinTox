#ifndef _POSTBOX_H_
#define _POSTBOX_H_

#ifdef __cplusplus
extern "C"
{
#endif

#include <threads.h>

#include "list.h"


/* Define a PBMTypes enum before including this
 * it should look like:
 *
 * typedef enum PBMTypes_e
 * {
 * 	PB_Typ1 =1,
 * 	PB_Typ2 =2
 * }
 *               PBMTypes;
 */

typedef struct PBMessage_s
{
	char *S1, *S2, *S3;
	int I1, I2, I3;
} PBMessage_t;

typedef struct PBRegistryEntry_s
{
	int mtypes;
	void (*callback)(int, PBMessage_t*, void*);
	void *custom;
} PBRegistryEntry_t;

typedef struct PBDeferred_s
{
	int mtype;
	PBMessage_t *msg;
} PBDeferred_t;

typedef struct Postbox_s
{
	List_t *clients;
	List_t *deferred;
	mtx_t Lock;
} Postbox_t;

Postbox_t *PB_New();
void PB_Defer(Postbox_t *pb, int mtype, PBMessage_t *msg);
void PB_Signal(Postbox_t *pb, int mtype, PBMessage_t* msg);
void PB_Despatch_Deferred(Postbox_t *pb);
void PB_Register(Postbox_t *pb, int mtypes, void*,
                 void (*callback)(int, PBMessage_t*, void*));

#ifdef __cplusplus
}
#endif

#endif
