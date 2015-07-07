#ifndef __MANAGER__H__
#define __MANAGER__H__

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

/* Private: */
	/* Pointer to the XWFramework Manager's Services structure. */
	XWF_Services_t psrvServices;
} ModuleManager_t;

ModuleManager_t *ModuleManager_getInstance();
void ModuleManager_init();
void ModuleManager_shutdown();

int ModuleManager_loadDynamicModule(const char *pszPath);
void *ModuleManager_createObject(const char *pszType);

/* Private: */
int ModuleManager_initialiseModule(XWF_Module_t *modNew, XWF_Init_f fnInit);
int ModuleManager_registerObject(const XWF_Object_t *pobjRegistered);
int ModuleManager_call(const char *pszService, void *pvParams);

#endif

