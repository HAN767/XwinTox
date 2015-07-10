#ifndef __GUIFLTK__H
#define __GUIFLTK__H

#include <threads.h>

#include "AOM/IGUICxx.h"
#include "Module/Module.h"
#include "misc.h"
#include "control/xwin.h"

class GUIFLTK : public GUICxxInterfaceT<GUIFLTK>
{
public:
	GUIFLTK(XWF_ObjectParams_t *pobjParams);
	int start();

private:
	static int fltkloop(void*);
	void launchfltkloop();
	XwinTox *Xw_; 
	thrd_t thrdFLTK_;
};

extern GUIFLTK *pgflCurrent;


#endif
