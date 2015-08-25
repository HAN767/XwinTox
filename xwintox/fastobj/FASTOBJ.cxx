#include <cstdio>
#include <cstring>
#include <sys/stat.h>
#include <threads.h>
#include <unistd.h>
#include "misc.h"
#include "hexstring.h"
#include "xwintox.h"
#include "AOM/IXWClass.h"
#include "Module/Module.h"
#include "FASTOBJ.h"

FASTOBJ::FASTOBJ (XWF_ObjectParams_t * prmObj) : Vm ("../lib")
{
    dbg ("New FASTOBJ created. Subtype: %s\n", prmObj->pszObjSubtype);

    if (prmObj->pszObjSubtype != 0 &&
        (strcmp (prmObj->pszObjSubtype, "INTER") == 0))
    {
        thrd_create (&thrdInteractive, (thrd_start_t)vmThread, this);
    }
}

int FASTOBJ::vmThread (void * customData)
{
    char buffer[255];
    FASTOBJ * self = static_cast< FASTOBJ * > (customData);

    while (1)
    {
        std::string line;
        std::string prompt ("{" + iToA (self->Vm.OStack.size ()) + "} ok ");
        fputs (prompt.c_str (), stdout);
        fflush (stdout);
        if (fgets (buffer, sizeof (buffer), stdin) == NULL)
            break;

        self->Vm.Eval (buffer);
    }

    return 0;
}