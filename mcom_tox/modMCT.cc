#include "AOM/IXWClass.h"
#include "MCOMMTOX.h"

extern "C" int XWF_exit()
{
	return 0;
}

extern "C" int XWF_init(XWF_Module_t *pmodSelf, const XWF_Services_t *psrvServices)
{
	XWF_Class_t *objMCOM =new XWF_Class_t;

	pmodSelf->pszName ="MComm for Tox";
	pmodSelf->fnExit =XWF_exit;

	objMCOM->pszType ="MESSENGER";
	objMCOM->pszSubtype ="TOX";
	objMCOM->enLang =XWF_Lang_CXX;
	objMCOM->pmodProvider =pmodSelf;
	objMCOM->fnCreate =MCOMMTOX::create;
	objMCOM->fnDestroy =MCOMMTOX::destroy;

	psrvServices->fnRegisterClass(objMCOM);
	return 0;
}
