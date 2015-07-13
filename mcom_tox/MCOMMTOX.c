#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <unistd.h>

#include <tox/tox.h>

#include "Module/Module.h"
#include "AOM/IMComm.h"
#include "xwintox.h"
#include "dictionary.h"
#include "misc.h"
#include "postbox.h"
#include "hexstring.h"

#include "MCOMMTOX.h"
#include "mctfuncs.h"

void default_config(Dictionary_t *conf);
MCT_Data_t loaddata(const char *szFile);

void *MCommTox_create(XWF_ObjectParams_t *pobpParams)
{
	IMComm_t *pimcNew =calloc(1, sizeof(IMComm_t));

	dbg("Initialising new MComm for Tox\n");

	pimcNew->lstContacts =List_new();
	pimcNew->dictConfig =Dictionary_new();
	pimcNew->pvPrivate =calloc(1, sizeof(MCommTox_Private_t));
	strncpy(pimcNew->szConfigFile, pobpParams->psrvServices->fnCall(
	            pobpParams->pobjhHandle,
	            "APP/GetConfigFilename", "toxconf"), 255);

	Dictionary_load_from_file(pimcNew->dictConfig,pimcNew->szConfigFile,1);
	default_config(pimcNew->dictConfig);

	strncpy(PRIVATE(pimcNew)->szToxSave, pobpParams->psrvServices->fnCall(
	        pobpParams->pobjhHandle, "APP/GetDataFilename", "toxsave"), 255);
	PRIVATE(pimcNew)->datToxSave =loaddata(PRIVATE(pimcNew)->szToxSave);

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
	fread(datRet.pbData, iLength, 1, hfSave);

	return datRet;
}

void getfriends(Tox *tox, List_t *lstFriends)
{
	unsigned int wCount =(tox_self_get_friend_list_size(tox));
	if (wCount < 1) return;
	unsigned int *wNums =calloc(wCount, sizeof(unsigned int));

	tox_self_get_friend_list(tox, wNums);

	for(int i =0; i < wCount; i++)
	{
		unsigned int wSize, wNum =wNums[i];
		char *pszText;
		unsigned char *pbPubkey;
		XWContact_t *ctNew =calloc(1, sizeof(XWContact_t));

		wSize =tox_friend_get_name_size(tox, wNum, 0) + 1;
		pszText =calloc(wSize, sizeof (char));
		tox_friend_get_name(tox, wNum, (uint8_t*)pszText, 0);
		pszText[wSize] ='\0';
		ctNew->pszName =pszText;

		wSize =tox_friend_get_status_message_size(tox, wNum, 0) + 1;
		pszText =calloc(wSize, sizeof (char));
		tox_friend_get_status_message(tox, wNum, (uint8_t*)pszText, 0);
		pszText[wSize] ='\0';
		ctNew->pszStatus =pszText;

		pbPubkey =malloc(TOX_PUBLIC_KEY_SIZE);
		tox_friend_get_public_key(tox, wNum, pbPubkey, 0);
		ctNew->pszID =bin_to_hex_string(pbPubkey, TOX_PUBLIC_KEY_SIZE);

		List_add(lstFriends, ctNew);
	}
}

int toxthread(void *custom)
{
	XWF_Object_Handle_t *hobjSelf =custom;
	PREP

	while (1)
	{
		LOCK(pimcSelf)
		tox_iterate(TOXINST);
		UNLOCK(pimcSelf)
		usleep(tox_iteration_interval(TOXINST) * 1000);
	}
}
