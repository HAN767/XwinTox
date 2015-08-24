#include "xwforth.h"
#include "xwfword.h"
#include "xwfvm.h"

void WInteger::Execute (class VM & Vm) { Vm.OPush (Duplicate ()); }
