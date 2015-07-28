#include "AOM/IXWClass.h"
#include "MCOMMTOX.h"
#include "toxdns3/TOXDNS3.h"

extern "C" int XWF_exit()
{
	return 0;
}

extern "C" int XWF_init(XWF_Module_t *pmodSelf, const XWF_Services_t *psrvServices)
{
	XWF_Class_t *objMCOM =new XWF_Class_t, *objTOX3 =new XWF_Class_t;

	pmodSelf->pszName ="MComm for Tox";
	pmodSelf->fnExit =XWF_exit;

	objMCOM->pszType ="MESSENGER";
	objMCOM->pszSubtype ="TOX";
	objMCOM->enLang =XWF_Lang_CXX;
	objMCOM->pmodProvider =pmodSelf;
	objMCOM->fnCreate =MCOMMTOX::create;
	objMCOM->fnDestroy =MCOMMTOX::destroy;

	objTOX3->pszType ="TOXDNS3RESOLVER";
	objTOX3->pszSubtype ="TOX";
	objTOX3->enLang =XWF_Lang_CXX;
	objTOX3->pmodProvider =pmodSelf;
	objTOX3->fnCreate =TOXDNS3::create;
	objTOX3->fnDestroy =TOXDNS3::destroy;

	psrvServices->fnRegisterClass(objMCOM);
	psrvServices->fnRegisterClass(objTOX3);
	return 0;
}
