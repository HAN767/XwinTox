#include <stdlib.h>

#include "Module/Module.h"
#include "AOM/IMComm.h"
#include "misc.h"

#include "IMCommTox.h"

int XWF_exit()
{
	return 0;
}

int XWF_init(XWF_Module_t *pmodSelf, const XWF_Services_t *psrvServices)
{
	XWF_Object_t *objIM =malloc(sizeof(XWF_Object_t));

	pmodSelf->pszName ="IMComm for Tox IM";
	pmodSelf->fnExit =XWF_exit;

	objIM->pszType ="IM";
	objIM->enLang =XWF_Lang_C;
	objIM->pmodProvider =pmodSelf;
	objIM->fnCreate =IMCommTox_create;
	objIM->fnDestroy =IMCommTox_destroy;

	psrvServices->fnRegisterObj(objIM);
	psrvSvcs =psrvServices;

	return 0;
}
