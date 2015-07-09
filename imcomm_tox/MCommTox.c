#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>

#include <tox/tox.h>

#include "Module/Module.h"
#include "AOM/IMComm.h"
#include "dictionary.h"
#include "misc.h"
#include "postbox.h"

#include "MCommTox.h"
#include "mctfuncs.h"

void default_config(Dictionary_t *conf);
MCT_Data_t loaddata(const char *szFile);

const XWF_Services_t *psrvSvcs;

void *MCommTox_create(XWF_ObjectParams_t *pobpParams)
{
	IMComm_t *pimcNew =calloc(1, sizeof(IMComm_t));

	dbg("Initialising new MComm for Tox\n");

	pimcNew->dictConfig =Dictionary_new();
	pimcNew->pvPrivate =calloc(1, sizeof(MCommTox_Private_t));
	strncpy(pimcNew->szConfigFile, pobpParams->psrvServices->fnCall(
	            pobpParams->pobjhHandle,
	            "APP/GetConfigFilename", "toxconf"), 255);

	Dictionary_load_from_file(pimcNew->dictConfig,pimcNew->szConfigFile,1);
	default_config(pimcNew->dictConfig);

	PRIVATE(pimcNew)->datToxSave =loaddata(pobpParams->psrvServices->fnCall(
	        pobpParams->pobjhHandle, "APP/GetDataFilename", "toxsave"));

	mtx_init(&PRIVATE(pimcNew)->mtxToxAccess, mtx_plain);

	pimcNew->fnConnect =MCT_Connect;

	return pimcNew;
}

int MCommTox_destroy(void *pvToDestroy)
{
	IMComm_t *pimcToDestroy =(IMComm_t*) pvToDestroy;

	if(!pimcToDestroy) return -1;
	else
	{
		dbg("Destroying an MComm for Tox\n");
		Dictionary_write_to_file(pimcToDestroy->dictConfig,
		                         pimcToDestroy->szConfigFile);
		Dictionary_delete(pimcToDestroy->dictConfig);
		free(PRIVATE(pimcToDestroy)->datToxSave.pbData);
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

MCT_Data_t loaddata(const char *szFile)
{
	static MCT_Data_t datRet;
	FILE *hfSave;
	struct stat st;
	off_t iLength;

	datRet.wLength =0;

	hfSave =fopen(szFile, "rb");

	if(hfSave == NULL)
	{
		dbg("Failed to open savefile %s\n", szFile);
		return datRet;
	}

	stat(szFile, &st);
	iLength =st.st_size;

	if(iLength == 0)
	{
		fclose(hfSave);
		dbg("Save file unusual: length is zero\n");
		return datRet;
	}

	datRet.pbData =malloc(iLength);
	datRet.wLength =iLength;
	fread(datRet.pbData, sizeof(datRet.pbData), 1, hfSave);

	return datRet;
}
