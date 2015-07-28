/* XwinTox
 *
 * The module manager's private interface lives here, as well as that interface
 * used by XwinTox the application but not by modules. */

#ifndef __MANAGER__H__
#define __MANAGER__H__

#include "postbox.h"
#include "list.h"
#include "dictionary.h"

#include "Module/Module.h"

/* First, the structure of a Module Manager instance. A note: module managers
 * are singletons. */
typedef struct ModuleManager_s
{
	/* Public: */
	/* Modules that have been loaded. */
	List_t *lstpmodModules;

	/* Objects which handle on specific type. */
	Dictionary_t *dictpobjObjects;
	/* Objects registered with the wildcard - they handle any type that an
	 * explicit class isn't registered for. */
	List_t *lstpobjWildcards;

	/* List of all modules that are registered. We have this list for the
	 * purpose of conveniently listing all modules registered, e.g. to modules
	 * that are interested in this knowledge. */
	List_t *lstAllMods;

	/* Private: */
	/* Pointer to the XWFramework Manager's Services structure. */
	XWF_Services_t psrvServices;
	/* Application-specific call handler. */
	XWF_Call_f fnAppCall;

	/* The global postbox. The entire modular system is built around
	 * asynchronous multithreaded communication through this postbox. */
	Postbox_t *pbGlobal;
} ModuleManager_t;

/* These functions get the (singleton) instance of the module manager,
 * initialise one, and destroy one, respectively. */
ModuleManager_t *ModuleManager_getInstance();
void ModuleManager_init(XWF_Call_f fnAppCall);
void ModuleManager_shutdown();

/* This loads a module that exists as a dynamically linked library. It returns
 * -1 for failure. */
int ModuleManager_loadDynamicModule(const char *pszPath);
int ModuleManager_loadStaticModule(XWF_Init_f fnInit);
/* Creates an object of type pszType. Returns null if no module includes a class
 * for such an object. */
XWF_Object_Handle_t *ModuleManager_createObject(const char *pszType);
/* And finally, this destroys an object. */
int ModuleManager_destroyObject(XWF_Object_Handle_t *pobjhToDelete);

/* Private - these ought only to be used within manager.c. Most of them are also
 * exported via the services structure to modules. */

/* This initialises a module. Returns -1 for a failure. */
int ModuleManager_initialiseModule_(XWF_Module_t *modNew, XWF_Init_f fnInit);
/* This registers a class with the module manager. */
int ModuleManager_registerClass_(const XWF_Class_t *pobjRegistered);
/* This calls one of the 'platform functions' exported to modules. */
void *ModuleManager_call_(const XWF_Object_Handle_t  *pobjhSource,
                          const char *pszService, const void *pvParams);
/* This lets a module subscribe to a signal of whatever type. */
int ModuleManager_subscribe_(const XWF_Object_Handle_t *hObject, int mtype,
                             void *custom, PB_Callback_f callback);
/* And this allows the module to dispatch signals. */
int ModuleManager_dispatch_(const XWF_Object_Handle_t *hObject, int iType,
                            PBMessage_t *ppbmMsg);
/* And finally, this allows a module to register a filter.*/
int ModuleManager_registerFilter_(const XWF_Object_Handle_t *hObject, int mtype,
                             void *custom, PB_Callback_f callback);
#endif
