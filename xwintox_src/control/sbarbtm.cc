#include "xwin/svgs.h"

#include "control/gui.h"
#include "control/sbarbtm.h"
#include "control/svgbox.h"

void transferspressed(Fl_Widget* B , void*)
{
	Xw->contents->NewCurrentArea(Xw->contents->transfers);
	Xw->sidebar->bottom_area->deselect_all();
	Xw->sidebar->contacts->deselect_all();
	Xw->sidebar->bottom_area->transfers->color(fl_rgb_color(68, 68, 67));
}

Sidebar_Bottom_Area::Sidebar_Bottom_Area(int S) : Fl_Group(Xw->basex * S,
	        Xw->h() - (36 * S) - (Xw->basey * S), (Xw->sblength * S),
	        Xw->h() - (Xw->basey * S))
{
	scale =S;

	box(FL_FLAT_BOX);
	color(fl_rgb_color(28, 28, 28));

	addfriend =new SVGBox(0, 0, 56 * scale, 36 * S, S, addfriendsvg, 0.5);
	newgroup =new SVGBox(0, 0, 112 * S , 36 * S, S, groupsvg, 0.5);
	transfers =new SVGBox(0, 0, 168 * S , 36 * S, S, transfersvg, 0.5);
	settings =new SVGBox(0, 0, 212 * S , 36 * S, S, settingssvg, 0.5);
	addfriend->show(); newgroup->show(); transfers->show();settings->show();

	transfers->callback(transferspressed);

	end();
}

void Sidebar_Bottom_Area::resize(int X, int Y, int W, int H)
{
	Fl_Group::resize(X, Y, W, H);
	addfriend->position(x(), Xw->h() - (36 * scale));
	newgroup->position(x() + (56 * scale), Xw->h() - (36 * scale));
	transfers->position(x() + (112 * scale), Xw->h() - (36 * scale));
	settings->position(x() + (168 * scale), Xw->h() - (36 * scale));
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
