#include "misc.h"

#include "nanosvg/svgs.h"
#include "control/sbarbtm.h"
#include "control/svgbox.h"
#include "control/xwin.h"

void transferspressed(Fl_Widget* B , void*)
{
	XwinTox *Xw =getXwin(B);

	Xw->contents->NewCurrentArea(Xw->contents->transfers);
	Xw->sidebar->bottom_area->deselect_all();
	Xw->sidebar->contacts->deselect_all();
	Xw->sidebar->bottom_area->transfers->color(fl_rgb_color(68, 68, 67));
}

void addfriendpressed(Fl_Widget* B , void*)
{
	XwinTox *Xw =getXwin(B);

	Xw->contents->NewCurrentArea(Xw->contents->addfriend);
	Xw->sidebar->bottom_area->deselect_all();
	Xw->sidebar->contacts->deselect_all();
	Xw->sidebar->bottom_area->addfriend->color(fl_rgb_color(68, 68, 67));
}

void settingspressed(Fl_Widget* B , void*)
{
	XwinTox *Xw =getXwin(B);

	Xw->contents->NewCurrentArea(Xw->contents->settings);
	Xw->sidebar->bottom_area->deselect_all();
	Xw->sidebar->contacts->deselect_all();
	Xw->sidebar->bottom_area->settings->color(fl_rgb_color(68, 68, 67));
}

Sidebar_Bottom_Area::Sidebar_Bottom_Area(const XWF_hObj_t* hObj, int S) :
	Fl_Group(0, 0, 1, 1)
{
	hObj_ =hObj;
	scale =S;

	box(FL_FLAT_BOX);
	color(fl_rgb_color(28, 28, 28));

	addfriend =new SVGBox(0, 0, 56 * scale, 36 * S, S, addfriendsvg, 0.5);
	newgroup =new SVGBox(0, 0, 56 * S , 36 * S, S, groupsvg, 0.5);
	transfers =new SVGBox(0, 0, 56 * S , 36 * S, S, transfersvg, 0.5);
	settings =new SVGBox(0, 0, 56 * S , 36 * S, S, settingssvg, 0.5);
	addfriend->show();
	newgroup->show();
	transfers->show();
	settings->show();

	addfriend->callback(addfriendpressed);
	transfers->callback(transferspressed);
	settings->callback(settingspressed);
	deselect_all();
	end();
}

void Sidebar_Bottom_Area::resize(int X, int Y, int W, int H)
{
	Fl_Group::resize(X, Y, W, H);
	addfriend->resize(X, Y, 56 * scale, 36 * scale);
	newgroup->resize(X + (56 * scale), Y, 56 * scale, H);
	transfers->resize(X + (112 * scale), Y, 56 * scale, H);
	settings->resize(X + (168 * scale), Y, 56 * scale, H);
}

void Sidebar_Bottom_Area::deselect_all()
{
	addfriend->box(FL_FLAT_BOX);
	addfriend->color(fl_rgb_color(28, 28, 28));
	transfers->box(FL_FLAT_BOX);
	transfers->color(fl_rgb_color(28, 28, 28));
	settings->box(FL_FLAT_BOX);
	settings->color(fl_rgb_color(28, 28, 28));
}
