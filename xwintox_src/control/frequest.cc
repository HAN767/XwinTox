#include <vector>
using namespace std;

#include <FL/Fl_Box.H>
#include <FL/Fl_Button.H>
#include <FL/Fl_Multiline_Output.H>

extern "C"
{
#include "xwintox.h"
}

#include "control/gui.h"
#include "control/frequest.h"

FriendRequests::FriendRequests(int X, int Y, int S)
	: Fl_Box(X, Y, (Xw->sblength * S) - 15, frheight * S)
{
	scale =S;
	box(FL_BORDER_BOX);
	color(fl_rgb_color(107, 194, 96));
}

void FriendRequests::show()
{
	((Sidebar*)parent())->top2_h +=frheight + 4;
	Fl_Box::show();
}

void FriendRequests::hide()
{
	((Sidebar*)parent())->top2_h -=frheight + 4;
	Fl_Box::hide();
}
