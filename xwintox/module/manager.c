/* XwinTox
 *
 * This file is home to the module manager implementation. */

#include <dlfcn.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "misc.h"
#include "postbox.h"
#include "dictionary.h"

#include "module/manager.h"


/* This points to the extant module manager. */
ModuleManager_t *pmmManager;

/* Remembering that the module manager is a singleton, this function returns a
 * pointer to the extant module manager instance, of which only one exists, */
ModuleManager_t *ModuleManager_getInstance()
{
	static ModuleManager_t mmManager;

	return &mmManager;
}

void ModuleManager_init(XWF_Call_f fnAppCall)
{
	/* Let's set this global variable for ease of access. */
	pmmManager =ModuleManager_getInstance();

	/* Prepare the global abtract datastructures. */
	pmmManager->pbGlobal =PB_New();
	pmmManager->lstpmodModules =List_new();
	pmmManager->lstpobjWildcards =List_new();
	pmmManager->dictpobjObjects =Dictionary_new();
	pmmManager->lstAllMods =List_new();

	/* Set the module manager version. Not presently used, but it might be some day.
	 */
	pmmManager->psrvServices.uiVersion =1;

	/* Set the platform-services structure's function pointers to their
	 * implementations here. */
	pmmManager->psrvServices.fnRegisterClass =ModuleManager_registerClass_;
	pmmManager->psrvServices.fnCall =ModuleManager_call_;
	pmmManager->psrvServices.fnSubscribe =ModuleManager_subscribe_;
	pmmManager->psrvServices.fnDispatch =ModuleManager_dispatch_;

	/* And, finally, we'll set fnAppCall, which is used by XwinTox to provide
	 * XwinTox-specific services. */
	pmmManager->fnAppCall =fnAppCall;
}

void ModuleManager_destroy()
{
	/* We should delete all the abstract data structures here.
	 * Deleting the lists is another matter, because the lists contain datatypes
	 * that we can't just free(); we need to expressly delete their members. */
	Dictionary_delete(pmmManager->dictpobjObjects);
}

int ModuleManager_loadDynamicModule(const char *pszPath)
{
	/* Naturally we need a new XWF_Module_t. */
	XWF_Module_t *pmodNew =malloc(sizeof(XWF_Module_t));
	const char *pszDLError;
	XWF_Init_f fnInit;

	pmodNew->enModtype =XWF_Dynamic;
	/* Set the shared object handle. */
	pmodNew->hdlLib =dlopen(pszPath, RTLD_LAZY | RTLD_GLOBAL);

	/* If we couldn't do that it's time to give up. */
	if((pszDLError =dlerror()) != NULL) goto dlerror;

	/* Find the initialisation function. Remember, we only need to explicitly
	 * look up just this one symbol; the module itself as well as other functions
	 * here in the manager will populate the rest of the datastructure. */
	fnInit =dlsym(pmodNew->hdlLib, "XWF_init");

	if((pszDLError =dlerror()) != NULL) goto dlerror;

	/* Let's try to initialise the module now. */
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

	/* Is there a class already set up for this? */
	if((pobjHandler =Dictionary_get_pointer
	                 (pmmManager->dictpobjObjects, pszType)) != 0)
	{
		XWF_Object_Handle_t *pobjhCreated =malloc(sizeof(XWF_Object_Handle_t));
		obpParams.pobjhHandle =pobjhCreated;
		pobjhCreated->pxwoClass =pobjHandler;
		pobjhCreated->pSvcs =&pmmManager->psrvServices;
		pobjhCreated->hObj =pobjHandler->fnCreate(&obpParams);

		/* There is, and it went well. An object was created. */
		if(pobjhCreated->hObj) return pobjhCreated;
		/* There is, but for some reason, it didn't create an object! */
		else free(pobjhCreated);
	}

	/* Later, we should try and find

		return 0; /* no module can create such an object */
}

int ModuleManager_destroyObject(XWF_Object_Handle_t *pobjhToDelete)
{
	/* We can't free an object that doesn't exist. */
	if(!pobjhToDelete) return -1;
	else
	{
		/* But through the classes own destroy function can we destroy one that does. */
		pobjhToDelete->pxwoClass->fnDestroy(pobjhToDelete->hObj);
		/* Class doesn't need to have the object handle. So we delete it ourselves. */
		free(pobjhToDelete);
		return 0;
	}
}

int ModuleManager_initialiseModule_(XWF_Module_t *pmodNew, XWF_Init_f fnInit)
{
	/* lstClasses will be populated later by any classes that the module registers
	 * with the manager.
	 * We don't actually need it since classes are first-class standalone objects.
	 * But it's useful to give a listing of classes that a specific module registers
	 * for whatever reason. */
	pmodNew->lstClasses =List_new();
	/* The module can try to initialise itself. */
	int iRet =fnInit(pmodNew, &pmmManager->psrvServices);

	if(!iRet)
	{
		/* It went perfectly, so we can print out a success message. */
		dbg("Module <%s> loaded successfully.\n", pmodNew->pszName);
		/* We can also add it to the list of all modules. */
		List_add(pmmManager->lstAllMods, pmodNew);
		return 0;
	}
	else if(iRet == 1)
	{
		/* It had some error, but none sufficient to warrant considering loading the
		 * module to have failed. */
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
	/* Give it a nice language name so we can print that out. */
	const char *pszLang =XWF_Lang_Text_sz[pobjRegistered->enLang];

	/* It registered with class name *, which indicates it will try to handle
	 * any class name. */
	if(strcmp(pobjRegistered->pszType, "*") == 0)
	{
		dbg("Adding new %s wildcard class provided by <%s>\n", pszLang,
		    pobjRegistered->pmodProvider->pszName);
		/* Add it to the wildcard classes list... */
		List_add(pmmManager->lstpobjWildcards, (void*)pobjRegistered);
		/* and also add it to its parent modules' list of provided classes. */
		List_add(pobjRegistered->pmodProvider->lstClasses,
		         (void*)pobjRegistered);
		return 0;
	}
	/* This class is already provided by another module. */
	else if(Dictionary_get(pmmManager->dictpobjObjects,
	                       pobjRegistered->pszType))
	{
		dbg("Object already exists: <%s>\n", pobjRegistered->pszType);
		return -1;
	}
	/* It's not a wildcard class, nor is it a class provided by another module. */
	else
	{
		dbg("Adding new %s class <%s> of subtype <%s> provided by <%s>\n",
		    pszLang, pobjRegistered->pszType, pobjRegistered->pszSubtype,
		    pobjRegistered->pmodProvider->pszName);
		/* So add it to the dictionary, which maps class names to their handlers. */
		Dictionary_set_pointer(pmmManager->dictpobjObjects,
		                       pobjRegistered->pszType, pobjRegistered);
		/* And, again, to the parent module's list of provided classes. */
		List_add(pobjRegistered->pmodProvider->lstClasses, (void*)pobjRegistered);
		return 0;
	}
}

void *ModuleManager_call_(const XWF_Object_Handle_t *pobjhSource,
                          const char *pszService,
                          const void *pvParams)
{
	/* A service with a slash not preceded by the name of some provider is a service
	 * provided by the module manager itself. */
	if(strncmp(pszService, "/", 1) == 0)
	{

		/* It wants a list of modules that are loaded. */
		if(strcmp(pszService, "/GETMODULESINFO") == 0)
		{
			List_t *lstRet =List_new();;

			/* Let us, then, open the list of all modules, and we'll add to
			 * lstRet an XWF_ModInfo_t to describe each. */
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

			/* Populate the new ModInfo structure with the appropriate data. */
			modinfoNew->hMod =modEntry;
			modinfoNew->pszName =strdup(modEntry->pszName);
			modinfoNew->pszType =strdup(XWF_Modtype_Text_sz[modEntry->enModtype]);
			modinfoNew->pszClasses =pszClasses;

			List_add(lstRet, modinfoNew);
			LIST_ITERATE_CLOSE(pmmManager->lstAllMods)

			/* Now we can give the caller that list. */
			return lstRet;
		}

		/* It wants a list of ClassInfos provided by some module. It gave us
		 * in pvParams an opaque handle to the module whose classes it is
		 * interested in. */
		else if(strcmp(pszService, "/GETMODULECLASSESINFO") == 0)
		{
			const XWF_Module_t *modThis =pvParams;
			List_t *lstRet =List_new();

			LIST_ITERATE_OPEN(modThis->lstClasses)
			XWF_Class_t *clsCur =e_data;
			XWF_ClassInfo_t *clsiNew=malloc(sizeof(XWF_ClassInfo_t));

			/* Just format nicely the name and language strings in the ClassInfo
			 * structure. */
			asprintf(&clsiNew->pszName, "%s.%s", clsCur->pszType,
			         clsCur->pszSubtype);
			asprintf(&clsiNew->pszLang, "%s", XWF_Lang_Text_sz[clsCur->enLang]);

			List_add(lstRet, clsiNew);
			LIST_ITERATE_CLOSE(modThis->lstClasses)

			return lstRet;
		}
	}

	/* It wants a function that's provided by the application, not the module
	 * manager. */
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
	/* Register with the global postbox the appropriate data. */
	PB_Register(pmmManager->pbGlobal, mtype, custom, callback);
	return 0;
}

int ModuleManager_dispatch_(const XWF_Object_Handle_t *hObject, int iType,
                            PBMessage_t *ppbmMsg)
{
	dbg("Object of class %s.%s emits signal %d\n", hObject->pxwoClass->pszType,
	    hObject->pxwoClass->pszSubtype, iType);
	/* Issue the signal. */
	PB_Signal_Multithreaded(pmmManager->pbGlobal, iType, ppbmMsg);
	return 0;
}
