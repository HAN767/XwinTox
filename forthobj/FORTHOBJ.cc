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
#include "FORTHOBJ.h"

FORTHOBJ::FORTHOBJ(XWF_ObjectParams_t *prmObj)
{
	dbg("New FORTHOBJ created\n");
	vm_ =ficlSystemCreateVm(globalSystem);
	ficlVmEvaluate (vm_, ".ver .( XwinTox Objective-Forth " __DATE__ " ) cr quit") ;
}

int FORTHOBJ::start()
{
	return 0;
}

