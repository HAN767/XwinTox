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

	box(FL_FLAT_BOX);
	color(fl_rgb_color(65, 65, 65));
}

void TransfersEntry::draw()
{
	Fl_Group::draw();
}

