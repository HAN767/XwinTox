#ifndef __MANAGER__H__
#define __MANAGER__H__

#include "postbox.h"
#include "list.h"
#include "dictionary.h"

#include "Module/Module.h"

typedef struct ModuleManager_s
{
	/* Public: */
	/* Modules that have been loaded. */
	List_t *lstpmodModules;

	/* Objects which handle on specific type. */
	Dictionary_t *dictpobjObjects;
	/* Objects registered with the wildcard - they handle any type. */
	List_t *lstpobjWildcards;

	/* List of module names, programming languages, and descriptions */
	List_t *lstModinfo;

	/* Private: */
	/* Pointer to the XWFramework Manager's Services structure. */
	XWF_Services_t psrvServices;
	/* Application-specific call handler */
	XWF_Call_f fnAppCall;
	/* Global postbox */
	Postbox_t *pbGlobal;
} ModuleManager_t;

ModuleManager_t *ModuleManager_getInstance();
void ModuleManager_init(XWF_Call_f fnAppCall);
void ModuleManager_shutdown();

int ModuleManager_loadDynamicModule(const char *pszPath);
XWF_Object_Handle_t *ModuleManager_createObject(const char *pszType);
int ModuleManager_destroyObject(XWF_Object_Handle_t *pobjhToDelete);

/* Private: */
int ModuleManager_initialiseModule_(XWF_Module_t *modNew, XWF_Init_f fnInit);
int ModuleManager_registerClass_(const XWF_Class_t *pobjRegistered);
void *ModuleManager_call_(const XWF_Object_Handle_t  *pobjhSource,
                          const char *pszService, const void *pvParams);
int ModuleManager_subscribe_(const XWF_Object_Handle_t *hObject, int mtype,
                              void *custom, PB_Callback_f callback);
int ModuleManager_dispatch_(const XWF_Object_Handle_t *hObject, int iType,
                            PBMessage_t *ppbmMsg);
#endif
