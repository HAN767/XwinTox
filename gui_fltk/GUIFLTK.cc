#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>

#include <FL/Fl.H>

#include "Module/Module.h"
#include "AOM/IGUICxx.h"
#include "dictionary.h"
#include "misc.h"
#include "postbox.h"

#include "GUIFLTK.h"
#include "control/xwin.h"

GUIFLTK *pgflCurrent;

GUIFLTK::GUIFLTK(XWF_ObjectParams_t *prmObj)
{
	dbg("Initialising a new GUI for FLTK\n");
	pgflCurrent =this;

	Xw_ =new XwinTox(prmObj->pobjhHandle, 640, 480, "XwinTox", 1);//1);
}

int GUIFLTK::start()
{
	Xw_->show();
	thrd_create(&thrdFLTK_, fltkloop, 0);
	return 0;
}

int GUIFLTK::fltkloop(void *)
{
	while(1)
	{
		Fl::lock();
		Fl::wait();
		Fl::unlock();
	}
	return 0;
}
