#include "control/gui.h"
#include "control/sidebar.h"

Sidebar::Sidebar(int S) : Fl_Group(Xw->basex * S,Xw->basey * S,
	                                   (Xw->sblength * S),
	                                   Xw->h() - (Xw->basey * S))
{
	scale =S;
	top2_h =38;

	box(FL_FLAT_BOX);
	color(fl_rgb_color(65, 65, 65));
	top_area =new Sidebar_Top_Area(S);
	bottom_area =new Sidebar_Bottom_Area(S);
	contacts =new ContactsList(x(), y() + ((60 + 36) * S),(Xw->sblength * S),
	                           h() - (36 * S) - (60 * S), S);
	contacts->end();

	frbutton =new Fl_Button(0, 0, 0, 0);
	frbutton->labelsize(10 * scale);
	frbutton->label("0 Friend\nRequests");
	frbutton->color(fl_rgb_color(107, 194, 96));
	frbutton->labelcolor(255);

	resize(x(), y(), w(), h());
	end();
}

void Sidebar::resize(int X, int Y, int W, int H)
{
	Fl_Group::resize(Xw->basex * scale,Xw->basey * scale,
	                                   (Xw->sblength * scale),
	                                   Xw->h() - (Xw->basey * scale));
	top_area->resize(Xw->basex * scale, Xw->basey * scale,
	                 Xw->sblength * scale, 60 * scale);
	bottom_area->resize(Xw->basex * scale,
	                    Xw->h() - (36 * scale) - (Xw->basey * scale),
	                    (Xw->sblength * scale),
	                    Xw->h() - (Xw->basey * scale));

	frbutton->resize(x() + w() - (72  * scale), y() + (64 * scale),
							60 * scale, 30 * scale);

	contacts->resize(x(), y() + ((60 + top2_h) * scale), (Xw->sblength * scale),
	                 h() - (36 * scale) - (60 * scale) - (top2_h * scale));
}
