#include <FL/Fl_Group.H>

#include "xwintox.h"

#include "control/gui.h"
#include "control/transent.h"

TransfersEntry::TransfersEntry(int X, int Y, int S, Contact_t *C, int CO)
	: Fl_Group(X, Y, 224 * S, 50 * S)
{
	scale =S;
	contact =C;
	col =CO;

	box(FL_FLAT_BOX);
	color(fl_rgb_color(65, 65, 65));
}

void TransfersEntry::draw()
{
	Fl_Group::draw();
}

