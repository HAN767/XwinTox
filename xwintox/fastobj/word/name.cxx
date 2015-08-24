#include "xwforth.h"
#include "xwfword.h"
#include "xwfvm.h"

void WName::Execute (class VM & Vm)
{
    if (!isExecutable)
        Vm.OPush (Duplicate ());
    else
    {
        Vm.ExecuteIdentifier (Name);
    }
}
