#include "control/gui.h"
#include "control/sidebar.h"

Sidebar::Sidebar(int S) : Fl_Group(Xw->basex * S,Xw->basey * S,
	                                   (Xw->sblength * S),
	                                   Xw->h() - (Xw->basey * S))
{
	scale =S;

	box(FL_FLAT_BOX);
	color(fl_rgb_color(65, 65, 65));
	top_area =new Sidebar_Top_Area(S);
	bottom_area =new Sidebar_Bottom_Area(S);
	contacts =new ContactsList(x(), y() + (60 * S),(Xw->sblength * S),
	                           h() - (36 * S) - (60 * S), S);

	resize(x(), y(), w(), h());
	end();
}

void Sidebar::resize(int X, int Y, int W, int H)
{
	Fl_Group::resize(X, Y, W, H);
	top_area->resize(Xw->basex * scale, Xw->basey * scale,
	                 Xw->sblength * scale, 60 * scale);
	bottom_area->resize(Xw->basex * scale,
	                    Xw->h() - (36 * scale) - (Xw->basey * scale),
	                    (Xw->sblength * scale),
	                    Xw->h() - (Xw->basey * scale));
	contacts->resize(x(), y() + (60 * scale), (Xw->sblength * scale),
	                 h() - (36 * scale) - (60 * scale));
}
