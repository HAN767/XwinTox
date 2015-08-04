#include <vector>
#include <threads.h>

#include "AOM/IXWClass.h"
#include "FORTHOBJ.h"

std::vector<FORTHOBJ*> instances;
ficlSystem *globalSystem;

extern "C" int FOBJ_exit()
{
	return 0;
}

extern "C" int FOBJ_init(XWF_Module_t *pmodSelf, const XWF_Services_t *psrvServices)
{
	XWF_Class_t *objMCOM =new XWF_Class_t;

	pmodSelf->pszName ="Forth VM for XwinTox";
	pmodSelf->fnExit =FOBJ_exit;

	objMCOM->pszType ="FORTHVM";
	objMCOM->pszSubtype ="DYN";
	objMCOM->enLang =XWF_Lang_Forth;
	objMCOM->pmodProvider =pmodSelf;
	objMCOM->fnCreate =FORTHOBJ::create;
	objMCOM->fnDestroy =FORTHOBJ::destroy;

	psrvServices->fnRegisterClass(objMCOM);

	globalSystem =ficlSystemCreate(NULL);
	ficlSystemCompileExtras(globalSystem);
	ficlDictionarySetPrimitive(ficlSystemGetDictionary (globalSystem), 
							   "XWFLOGS", FORTHOBJ::wordXWFPRINTLOGS, 
								FICL_WORD_DEFAULT);
	ficlDictionarySetPrimitive(ficlSystemGetDictionary (globalSystem), 
							   "XWFSETCALLBACK", FORTHOBJ::wordXWFSETCALLBACK, 
								FICL_WORD_DEFAULT);
	ficlDictionarySetPrimitive(ficlSystemGetDictionary (globalSystem), 
							   "XWFSUBSCRIBE", FORTHOBJ::wordXWFSUBSCRIBE, 
								FICL_WORD_DEFAULT);
	return 0;
}
