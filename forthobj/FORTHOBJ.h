#ifndef __GUIFLTK__H
#define __GUIFLTK__H

#include <threads.h>

#include "misc.h"
#include "ficl.h"

#include "Module/Module.h"
#include "AOM/IXWClass.h"

class FORTHOBJ : public XWClassT<FORTHOBJ>
{
public:
	FORTHOBJ(XWF_ObjectParams_t *pobjParams);
	int start();

private:
	ficlVm *vm_;
};

extern ficlSystem *globalSystem;


#endif
