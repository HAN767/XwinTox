#include <stdlib.h>

#include "Module/Module.h"
#include "AOM/IGUI.h"
#include "misc.h"

#include "GUIFLTK.h"

extern "C" int XWF_exit()
{
	return 0;
}

extern "C" int XWF_init(XWF_Module_t *pmodSelf, const XWF_Services_t *psrvServices)
{
	XWF_Object_t *objGUI =new XWF_Object_t;

	pmodSelf->pszName ="GUI for FLTK";
	pmodSelf->fnExit =XWF_exit;

	objGUI->pszType ="GUI";
	objGUI->enLang =XWF_Lang_CXX;
	objGUI->pmodProvider =pmodSelf;
	objGUI->fnCreate =GUIFLTK::create;
	objGUI->fnDestroy =GUIFLTK::destroy;

	psrvServices->fnRegisterObj(objGUI);
	return 0;
}
