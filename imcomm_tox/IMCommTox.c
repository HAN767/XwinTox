#include <stdlib.h>
#include <string.h>

#include "Module/Module.h"
#include "AOM/IMComm.h"
#include "dictionary.h"
#include "misc.h"
#include "postbox.h"

#include "IMCommTox.h"

void default_config(Dictionary_t *conf);

void *IMCommTox_create(XWF_ObjectParams_t *pobpParams)
{
	IMComm_t *pimcNew =calloc(1, sizeof(IMComm_t));

	dbg("Initialising new MComm for Tox\n");

	pimcNew->pbEvents =PB_New();
	pimcNew->dictConfig =Dictionary_new();
	pimcNew->pszConfigFilename =strdup(pobpParams->psrvServices->fnCall(pimcNew,
	                                   "APP/GetConfigFilename", "toxconf"));

	Dictionary_load_from_file(pimcNew->dictConfig,pimcNew->pszConfigFilename,1);
	default_config(pimcNew->dictConfig);
	return pimcNew;
}

int IMCommTox_destroy(void *pvToDestroy)
{
	IMComm_t *pimcToDestroy =(IMComm_t*) pvToDestroy;

	if(!pimcToDestroy) return -1;
	else
	{
		dbg("Destroying an MComm for Tox\n");
		Dictionary_write_to_file(pimcToDestroy->dictConfig,
		                         pimcToDestroy->pszConfigFilename);
		Dictionary_delete(pimcToDestroy->dictConfig);
		free(pimcToDestroy->pszConfigFilename);
		free(pimcToDestroy);
		return 0;
	}
}

void default_config(Dictionary_t *conf)
{
#define Add(x,y) Dictionary_set_aux(conf, x, y)
	Add("Tox.BootstrapIP", "23.226.230.47");
	Add("Tox.BootstrapPort", "33445");
	Add("Tox.BootstrapKey",
	    "A09162D68618E742FFBCA1C2C70385E6679604B2D80EA6E84AD0996A1AC8A074");
}
