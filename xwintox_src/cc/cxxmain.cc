#include <threads.h>
#include <unistd.h>

#include <FL/Fl.H>

#include "xwintox_win.h"

extern "C"
{
#include "misc.h"
#include "dictionary.h"

#include "etc.h"
#include "xwintox.h"
}

void CommWork()
{
	mtx_lock (&APP->Comm->WorkMtx);
	APP->Comm->Work =1;
	cnd_broadcast(&APP->Comm->WorkCnd);
	mtx_unlock (&APP->Comm->WorkMtx);
	usleep(250);
	APP->Comm->Work =0;
}

extern "C" int CXXMain()
{
	int scale =2;
	/*sleep(5);

	APP->Comm->WantQuit=1;
	CommWork();*/
	Fl::visual(FL_RGB);
	XwinTox* Xwin = new XwinTox(640 * scale, 480 * scale, "XwinTox", scale);
	Xwin->show();

	Fl::run();

	return 0;
}
