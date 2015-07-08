#ifndef __IMCOMMTOX__H
#define __IMCOMMTOX__H

#include <tox/tox.h>

#include "Module/Module.h"

typedef struct IMCommTox_Private_s
{
	Tox *Tox;
} IMCommTox_Private_t;

void *IMCommTox_create(XWF_ObjectParams_t *pobpParams);
int IMCommTox_destroy(void *pobjToDestroy);

#endif
