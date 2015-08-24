#include "xwforth.h"
#include "xwfword.h"
#include "xwfvm.h"

void WArray::Execute (class VM & Vm) { Vm.OPush (Duplicate ()); }

void WArray::OpForAll (class VM & Vm, WordPtr Proc)
{
    for (std::vector< WordPtr >::reverse_iterator cell = Array.rbegin ();
         cell != Array.rend (); cell++)
    {
        Vm.OPush (*cell);
        Proc->Execute (Vm);
    }
}
