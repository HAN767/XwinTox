#include "xwforth.h"
#include "xwfword.h"
#include "xwfvm.h"

void WReal::Execute (class VM & Vm) { Vm.OPush (Duplicate ()); }
