#include <FL/Fl_Group.H>

#include "xwintox.h"

#include "control/gui.h"
#include "control/gtrnsfer.h"
#include "control/transent.h"

TransfersEntry::TransfersEntry(int X, int Y, int S, Transfer_t *T, int I)
	: Fl_Group(X, Y, Xw->w() - (Xw->sblength * S), 50 * S)
{
	scale =S;
	transfer =T;
	inv =I;

	accept =new Fl_Button(0, 0, 1, 1, "Accept");
	reject =new Fl_Button(0, 0, 1, 1, "Reject");

	accept->color(fl_rgb_color(118, 202, 116));
	reject->color(fl_rgb_color(214, 78, 77));

	box(FL_FLAT_BOX);

	if(I == 1) color(fl_rgb_color(198, 199, 214));
	else color(fl_rgb_color(239, 239, 239));

	resize(X, Y, w(), h());
	end();
}

void TransfersEntry::resize(int X, int Y, int W, int H)
{
	Fl_Group::resize(X, Y, W, H);
	accept->resize(X + w() - (78 * scale), y() + (2 * scale), 54 * scale,
	               (h() / 2) - 2 * scale);
	reject->resize(X + w() - (78 * scale), y() + (h() / 2) + (2 * scale),
	               54 * scale,(h() / 2) - 4 * scale);
}

void TransfersEntry::draw()
{
	Fl_Group::draw();
}
