#include <vector>
#include <threads.h>

#include "AOM/IXWClass.h"
//#include "LUAOBJ.h"

// std::vector<FORTHOBJ*> instances;
extern "C" int LOBJ_exit () { return 0; }

extern "C" int LOBJ_init (XWF_Module_t * pmodSelf,
                          const XWF_Services_t * psrvServices)
{
    XWF_Class_t * objMCOM = new XWF_Class_t;

    pmodSelf->pszName = "Lua VM for XwinTox";
    pmodSelf->fnExit = LOBJ_exit;

    objMCOM->pszType = "LUAOBJ";
    objMCOM->pszSubtype = "DYN";
    objMCOM->enLang = XWF_Lang_Lua;
    objMCOM->pmodProvider = pmodSelf;
    // objMCOM->fnCreate =FORTHOBJ::create;
    // objMCOM->fnDestroy =FORTHOBJ::destroy;

    psrvServices->fnRegisterClass (objMCOM);

    return 0;
}
