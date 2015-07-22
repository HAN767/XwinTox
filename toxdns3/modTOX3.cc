#include "AOM/IXWClass.h"
#include "TOXDNS3.h"

extern "C" int XWF_exit()
{
	return 0;
}

extern "C" int XWF_init(XWF_Module_t *pmodSelf, const XWF_Services_t *psrvServices)
{
	XWF_Class_t *objMCOM =new XWF_Class_t;

	pmodSelf->pszName ="Friend resolver for ToxDNS v3";
	pmodSelf->fnExit =XWF_exit;

	objMCOM->pszType ="TOXDNS3RESOLVER";
	objMCOM->pszSubtype ="TOX";
	objMCOM->enLang =XWF_Lang_CXX;
	objMCOM->pmodProvider =pmodSelf;
	objMCOM->fnCreate =TOXDNS3::create;
	objMCOM->fnDestroy =TOXDNS3::destroy;

	psrvServices->fnRegisterClass(objMCOM);
	return 0;
}
