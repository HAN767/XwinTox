#include "xwforth.h"
#include "xwfword.h"
#include "xwfvm.h"

void WLambda::Execute (class VM & Vm)
{
    content.append (" ");
    Vm.Eval (content.c_str ());
}
