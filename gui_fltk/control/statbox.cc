#include <FL/Fl.H>
#include <FL/Fl_Box.H>
#include <FL/fl_draw.H>

#include "xwintox.h"
#include "misc.h"
#include "Module/Module.h"

#include "control/statbox.h"

void statbox_recv(int iType, PBMessage_t* msg, void* custom)
{
	StatusBox *self =(StatusBox*) custom;
	if (iType == clConn)
	{
		PBMessage_t *msgNew =PB_New_Message();
		self->hObj_->pSvcs->fnDispatch(self->hObj_, clSaveData, msgNew);
		Fl::lock();
		self->redraw();
		self->status =msg->I1;
		Fl::unlock();
	}
}

StatusBox::StatusBox(const XWF_hObj_t* hObj, int S) : Fl_Box(0, 0, 1, 1)
{
	hObj_ =hObj;
	scale =S;
	box(FL_FLAT_BOX);
	color(fl_rgb_color(65, 65, 65));

	hObj->pSvcs->fnSubscribe(hObj, clConn, this, statbox_recv);
}

void StatusBox::draw()
{
	Fl_Box::draw();
	if (status == 1) fl_color(2);
	else fl_color(fl_rgb_color(230, 70, 74));
	fl_pie(this->x() + (5 * scale), this->y() + (15 * scale), 10 * scale,
	       10 * scale, 0, 360);
}
