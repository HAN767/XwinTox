#include <vector>
#include <threads.h>

#include "AOM/IXWClass.h"
//#include "LUAOBJ.h"

// std::vector<FORTHOBJ*> instances;
extern "C" int FOBJ_exit () { return 0; }

extern "C" int FOBJ_init (XWF_Module_t * pmodSelf,
                          const XWF_Services_t * psrvServices)
{
    XWF_Class_t * objMCOM = new XWF_Class_t;

    pmodSelf->pszName = "XwinFast VM";
    pmodSelf->fnExit = FOBJ_exit;

    objMCOM->pszType = "FASTLOBJ";
    objMCOM->pszSubtype = "DYN";
    objMCOM->enLang = XWF_Lang_Forth;
    objMCOM->pmodProvider = pmodSelf;
    // objMCOM->fnCreate =FORTHOBJ::create;
    // objMCOM->fnDestroy =FORTHOBJ::destroy;

    psrvServices->fnRegisterClass (objMCOM);

    return 0;
}
