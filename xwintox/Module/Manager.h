#ifndef __MANAGER__H__
#define __MANAGER__H__

#include "list.h"
#include "dictionary.h"

typedef struct ModuleManager_s
{
	List_t *lstpmodModules;

	/* dictionary(store addresses of object_t's as sprintf %ul'd strings? 
	  maybe list_t, schlower) of non-wildcar objects */
	Dictionary_t *dictpobjObjects;
	List_t *lstpobjWildcards;
} ModuleManager_t;
	

#endif

