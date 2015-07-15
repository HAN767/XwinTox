#include <FL/Fl.H>
#include <FL/fl_draw.H>

#include "misc.h"
#include "postbox.h"

#include "control/xwin.h"
#include "control/gsetting.h"

GSettings::GSettings(const XWF_hObj_t* hObj, int S) :
	GArea(hObj, S, "Settings")
{
	scale = S;

	end();
}

void GSettings::resize(int X, int Y, int W, int H)
{
	Fl_Group::resize(X, Y, W, H);
}

void GSettings::draw()
{
	GArea::draw();
}
