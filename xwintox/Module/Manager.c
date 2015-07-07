#include <string.h>

#include "misc.h"

#include "Module/Manager.h"

ModuleManager_t *pmmManager;

ModuleManager_t *ModuleManager_getInstance()
{
	static ModuleManager_t mmManager;

	return &mmManager;
}

void ModuleManager_init()
{
	pmmManager =ModuleManager_getInstance();

	pmmManager->lstpmodModules =List_new();
	pmmManager->lstpobjWildcards =List_new();
	pmmManager->dictpobjObjects =Dictionary_new();

	pmmManager->psrvServices.uiVersion =1;
	pmmManager->psrvServices.fnRegisterObj =ModuleManager_registerObject;
	pmmManager->psrvServices.fnCall =ModuleManager_call;
}

int ModuleManager_initialiseModule(XWF_Module_t *modNew, XWF_Init_f fnInit)
{
	int iRet =fnInit(modNew, &pmmManager->psrvServices);

	if(!iRet)
	{
		dbg("Module %s loaded successfully.\n", modNew->pszName);
		return 0;
	}
	else if(iRet == 1)
	{
		dbg("Module %s loaded with errors.\n", modNew->pszName);
		return 0;
	}
	else
	{
		dbg("Module failed to load.\n");
		return -1;
	}
}

int ModuleManager_registerObject(const XWF_Object_t *pobjRegistered)
{
	if(strcmp(pobjRegistered->pszType, "*") == 0)
	{
		dbg("Adding new wildcard object\n");
		List_add(pmmManager->lstpobjWildcards, (void*)pobjRegistered);
		return 0;
	}
	else if(Dictionary_get(pmmManager->dictpobjObjects, pobjRegistered->pszType))
	{
		dbg("Object already exists: %s\n", pobjRegistered->pszType);
		return -1;
	}
	else
	{
		dbg("Adding new object: %s\n", pobjRegistered->pszType);
		return 0;
	}
}

int ModuleManager_call(const char *pszService, void *pvParams)
{
	if(strncmp(pszService, "SYSTEM", 6) == 0)
	{
		dbg("System call: %s\n", pszService);
		return 0;
	}

	return -1;
}
