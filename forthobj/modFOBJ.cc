#include <threads.h>

#include "AOM/IXWClass.h"
#include "FORTHOBJ.h"

ficlSystem *globalSystem;
ficlVm *interactiveVm;
thrd_t interactiveThrd;

int vmThread(void *customData)
{
    char buffer[256];
	int returnValue =0;
	ficlVmEvaluate (interactiveVm, 
					".( XwinTox Objective-Forth Command Interpreter " 
					__DATE__ " ) cr quit");
    while (returnValue != FICL_VM_STATUS_USER_EXIT) 
	{
        fputs("ok ", stdout);
        fflush(stdout);

        if (fgets(buffer, sizeof(buffer), stdin) == NULL) break;
        returnValue = ficlVmEvaluate(interactiveVm, buffer);
    }
	return 0;
}

extern "C" int XWF_exit()
{
	return 0;
}

extern "C" int XWF_init(XWF_Module_t *pmodSelf, const XWF_Services_t *psrvServices)
{
	XWF_Class_t *objMCOM =new XWF_Class_t;

	pmodSelf->pszName ="Forth VM for XwinTox";
	pmodSelf->fnExit =XWF_exit;

	objMCOM->pszType ="FORTHVM";
	objMCOM->pszSubtype ="DYN";
	objMCOM->enLang =XWF_Lang_Forth;
	objMCOM->pmodProvider =pmodSelf;
	objMCOM->fnCreate =FORTHOBJ::create;
	objMCOM->fnDestroy =FORTHOBJ::destroy;

	psrvServices->fnRegisterClass(objMCOM);

	globalSystem =ficlSystemCreate(NULL);
	ficlSystemCompileExtras(globalSystem);

	interactiveVm =ficlSystemCreateVm(globalSystem);
	thrd_create(&interactiveThrd, vmThread, 0);

	return 0;
}
