#include <stdlib.h>

#include "Module/Module.h"
#include "AOM/IMComm.h"
#include "misc.h"
#include "postbox.h"

#include "IMCommTox.h"

void *IMCommTox_create(XWF_ObjectParams_t *pobpParams)
{
	IMComm_t *pimcNew =calloc(1, sizeof(IMComm_t));

	pimcNew->pbEvents =PB_New();
	return pimcNew;
}

int IMCommTox_destroy(void *pobjToDestroy)
{
	if(!pobjToDestroy) return -1;
	else
	{
		free(pobjToDestroy);
		return 0;
	}
}
