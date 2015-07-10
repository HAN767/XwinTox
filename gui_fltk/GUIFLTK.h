#ifndef __GUIFLTK__H
#define __GUIFLTK__H

#include <threads.h>

#include "AOM/IGUICxx.h"
#include "Module/Module.h"
#include "misc.h"

class GUIFLTK : public GUICxxInterfaceT<GUIFLTK>
{
public:
	GUIFLTK(XWF_ObjectParams_t *pobjParams);
	int start();

private:
	thrd_t thrdGUI_;
};

#endif
