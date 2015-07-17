#include <dlfcn.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "misc.h"
#include "postbox.h"
#include "dictionary.h"

#include "module/manager.h"

ModuleManager_t *pmmManager;

ModuleManager_t *ModuleManager_getInstance()
{
	static ModuleManager_t mmManager;

	return &mmManager;
}

void ModuleManager_init(XWF_Call_f fnAppCall)
{
	pmmManager =ModuleManager_getInstance();

	pmmManager->pbGlobal =PB_New();
	pmmManager->lstpmodModules =List_new();
	pmmManager->lstpobjWildcards =List_new();
	pmmManager->dictpobjObjects =Dictionary_new();
	pmmManager->lstAllMods =List_new();
	pmmManager->psrvServices.uiVersion =1;
	pmmManager->psrvServices.fnRegisterClass =ModuleManager_registerClass_;
	pmmManager->psrvServices.fnCall =ModuleManager_call_;
	pmmManager->psrvServices.fnSubscribe =ModuleManager_subscribe_;
	pmmManager->psrvServices.fnDispatch =ModuleManager_dispatch_;
	pmmManager->fnAppCall =fnAppCall;
}

void ModuleManager_destroy()
{
	Dictionary_delete(pmmManager->dictpobjObjects);
}

int ModuleManager_loadDynamicModule(const char *pszPath)
{
	XWF_Module_t *pmodNew =malloc(sizeof(XWF_Module_t));
	const char *pszDLError;
	XWF_Init_f fnInit;

	pmodNew->enModtype =XWF_Dynamic;
	pmodNew->hdlLib =dlopen(pszPath, RTLD_LAZY | RTLD_GLOBAL);

	if((pszDLError =dlerror()) != NULL) goto dlerror;

	fnInit =dlsym(pmodNew->hdlLib, "XWF_init");

	if((pszDLError =dlerror()) != NULL) goto dlerror;

	if(ModuleManager_initialiseModule_(pmodNew, fnInit) != 0) goto error;

	return 0;

dlerror:
	dbg("Failed to load dynamic module (%s)\n", pszDLError);
error:
	dbg("Unloading dynamic module (%s) due to error.\n", pszPath);
	free(pmodNew);
	return -1;
}

XWF_Object_Handle_t *ModuleManager_createObject(const char *pszType)
{
	XWF_ObjectParams_t obpParams;
	XWF_Class_t *pobjHandler;

	obpParams.pszObjType =pszType;
	obpParams.psrvServices =&pmmManager->psrvServices;

	if((pobjHandler =Dictionary_get_pointer
	                 (pmmManager->dictpobjObjects, pszType)) != 0)
	{
		XWF_Object_Handle_t *pobjhCreated =malloc(sizeof(XWF_Object_Handle_t));
		obpParams.pobjhHandle =pobjhCreated;
		pobjhCreated->pxwoClass =pobjHandler;
		pobjhCreated->pSvcs =&pmmManager->psrvServices;
		pobjhCreated->hObj =pobjHandler->fnCreate(&obpParams);

		if(pobjhCreated->hObj) return pobjhCreated;
		else free(pobjhCreated);
	}

	return 0; /* no module can create such an object */
}

int ModuleManager_destroyObject(XWF_Object_Handle_t *pobjhToDelete)
{
	if(!pobjhToDelete) return -1;
	else
	{
		pobjhToDelete->pxwoClass->fnDestroy(pobjhToDelete->hObj);
		free(pobjhToDelete);
		return 0;
	}
}

int ModuleManager_initialiseModule_(XWF_Module_t *pmodNew, XWF_Init_f fnInit)
{
	pmodNew->lstClasses =List_new();
	int iRet =fnInit(pmodNew, &pmmManager->psrvServices);

	if(!iRet)
	{
		dbg("Module <%s> loaded successfully.\n", pmodNew->pszName);
		List_add(pmmManager->lstAllMods, pmodNew);
		return 0;
	}
	else if(iRet == 1)
	{
		dbg("Module <%s> loaded with errors.\n", pmodNew->pszName);
		List_add(pmmManager->lstAllMods, pmodNew);
		return 0;
	}
	else
	{
		dbg("Module failed to load.\n");
		return -1;
	}
}

int ModuleManager_registerClass_(const XWF_Class_t *pobjRegistered)
{
	const char *pszLang ="Unknown";

	pszLang =XWF_Lang_Text_sz[pobjRegistered->enLang];

	if(strcmp(pobjRegistered->pszType, "*") == 0)
	{
		dbg("Adding new %s wildcard class provided by <%s>\n", pszLang,
		    pobjRegistered->pmodProvider->pszName);
		List_add(pmmManager->lstpobjWildcards, (void*)pobjRegistered);
		List_add(pobjRegistered->pmodProvider->lstClasses, (void*)pobjRegistered);
		return 0;
	}
	else if(Dictionary_get(pmmManager->dictpobjObjects,
	                       pobjRegistered->pszType))
	{
		dbg("Object already exists: <%s>\n", pobjRegistered->pszType);
		return -1;
	}
	else
	{
		dbg("Adding new %s class <%s> of subtype <%s> provided by <%s>\n",
		    pszLang, pobjRegistered->pszType, pobjRegistered->pszSubtype,
		    pobjRegistered->pmodProvider->pszName);
		Dictionary_set_pointer(pmmManager->dictpobjObjects,
		                       pobjRegistered->pszType, pobjRegistered);
		List_add(pobjRegistered->pmodProvider->lstClasses, (void*)pobjRegistered);
		return 0;
	}
}

void *ModuleManager_call_(const XWF_Object_Handle_t *pobjhSource,
                          const char *pszService,
                          const void *pvParams)
{
	if(strncmp(pszService, "/", 1) == 0)
	{
		dbg("Service\n");
		if (strcmp(pszService, "/GETMODULESINFO") == 0)
		{
			dbg("Prepare a list\n");
			List_t *lstRet =List_new();;

			LIST_ITERATE_OPEN(pmmManager->lstAllMods)
				XWF_ModInfo_t *modinfoNew =malloc(sizeof(XWF_ModInfo_t));
				XWF_Module_t *modEntry =(XWF_Module_t *)e_data;
				char *pszClasses =malloc(128);
				unsigned int dwRemLen =128;

				LIST_ITERATE_OPEN(modEntry->lstClasses)
					XWF_Class_t *clsCur =e_data;
					if((strlen(clsCur->pszSubtype) + strlen(clsCur->pszSubtype)
						+ 1) < dwRemLen)
					{
						dwRemLen -=sprintf(pszClasses, "%s.%s", clsCur->pszType,
											clsCur->pszSubtype);
					}
				LIST_ITERATE_CLOSE(modEntry->lstClasses)

				modinfoNew->hMod =modEntry;
				modinfoNew->pszName =strdup(modEntry->pszName);
				modinfoNew->pszType =strdup(XWF_Modtype_Text_sz[modEntry->enModtype]);
				modinfoNew->pszClasses =pszClasses;

				List_add(lstRet, modinfoNew);
			LIST_ITERATE_CLOSE(pmmManager->lstAllMods)

			return lstRet;
		}
		else if (strcmp(pszService, "/GETMODULECLASSESINFO") == 0)
		{
			dbg("Get Moduel Classes Info\n");
			const XWF_Module_t *modThis =pvParams;
			List_t *lstRet =List_new();

			LIST_ITERATE_OPEN(modThis->lstClasses)
				XWF_Class_t *clsCur =e_data;
				XWF_ClassInfo_t *clsiNew=malloc(sizeof(XWF_ClassInfo_t));

				asprintf(&clsiNew->pszName, "%s.%s", clsCur->pszType, clsCur->pszSubtype);
				asprintf(&clsiNew->pszLang, "%s", XWF_Lang_Text_sz[clsCur->enLang]);

				List_add(lstRet, clsiNew);
			LIST_ITERATE_CLOSE(modThis->lstClasses)

			return lstRet;
		}
	}
	if(strncmp(pszService, "APP", 3) == 0)
	{
		return pmmManager->fnAppCall(pobjhSource, pszService + 4, pvParams);
	}

	return 0;
}

int ModuleManager_subscribe_(const XWF_Object_Handle_t *hObject, int mtype,
                             void *custom, PB_Callback_f callback)
{
	dbg("Object of class %s.%s subscribed to signal %d\n",
	    hObject->pxwoClass->pszType, hObject->pxwoClass->pszSubtype,  mtype);
	PB_Register(pmmManager->pbGlobal, mtype, custom, callback);
	return 0;
}

int ModuleManager_dispatch_(const XWF_Object_Handle_t *hObject, int iType,
                            PBMessage_t *ppbmMsg)
{
	dbg("Object of class %s.%s emits signal %d\n", hObject->pxwoClass->pszType,
	    hObject->pxwoClass->pszSubtype, iType);
	PB_Signal_Multithreaded(pmmManager->pbGlobal, iType, ppbmMsg);
	return 0;
}
