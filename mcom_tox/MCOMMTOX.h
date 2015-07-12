#ifndef __IMCOMMTOX__H
#define __IMCOMMTOX__H

#include <threads.h>
#include <tox/tox.h>

#include "Module/Module.h"

typedef struct MCT_Data_s
{
	char *pbData;
	unsigned int wLength;
} MCT_Data_t;

typedef struct MCommTox_Private_s
{
	mtx_t mtxToxAccess;
	Tox *ptoxTox;
	char szToxSave[255];
	MCT_Data_t datToxSave;
} MCommTox_Private_t;

void *MCommTox_create(XWF_ObjectParams_t *pobpParams);
int MCommTox_destroy(void *pobjToDestroy);

/* internally used */
#define PRIVATE(IMC) ((MCommTox_Private_t*)IMC->pvPrivate)
#define LOCK(IMC) mtx_lock(&PRIVATE(IMC)->mtxToxAccess);
#define UNLOCK(IMC) mtx_unlock(&PRIVATE(IMC)->mtxToxAccess);

#endif
