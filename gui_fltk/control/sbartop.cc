#include <string.h>
#include <FL/Fl_Group.H>
#include <FL/Fl_Input.H>

#include "misc.h"
#include "nanosvg/svgs.h"
#include "control/sbartop.h"
#include "control/svgbox.h"
#include "control/statbox.h"

void namechangecb(Fl_Widget *w, long which);

Sidebar_Top_Area::Sidebar_Top_Area(const XWF_hObj_t* hObj, int S) :
	Fl_Group(0, 0, 1, 1)
{
	hObj_ =hObj;
	scale =S;

	box(FL_FLAT_BOX);
	color(fl_rgb_color(28, 28, 28));

	statusbox =new StatusBox(hObj_, S);
	avbox =new SVGBox(0, 0, 40 * scale, 40 * scale, S,
	                  default_av, 1);
	name =new Fl_Input(0, 0, 1, 1);
	status =new Fl_Input(0, 0, 1, 1);

	avbox->box(FL_ENGRAVED_BOX);
	avbox->color(fl_rgb_color(130, 128, 133));

	status->textsize(10 * S);
	status->box(FL_FLAT_BOX);
	status->color(fl_rgb_color(28, 28, 28));
	status->textcolor(54);
	status->when(FL_WHEN_RELEASE | FL_WHEN_ENTER_KEY);
	status->value("XwinTox Frameworks 2");//Dictionary_get(APP->Config, "Tox.Status"));
	status->callback(namechangecb);

	name->textsize(12 * S);
	name->box(FL_FLAT_BOX);
	name->color(fl_rgb_color(28, 28, 28));
	name->textcolor(255);
	name->value("XwinTox User");//Dictionary_get(APP->Config, "Tox.Name"));
	name->when(FL_WHEN_RELEASE | FL_WHEN_ENTER_KEY);
	name->callback(namechangecb, 1);

	avbox->show();

	end();
}

void Sidebar_Top_Area::resize(int X, int Y, int W, int H)
{
	//dbg("X: %d Y: %d W: %d H: %d\n", X, Y, W, H);
	Fl_Group::resize(X, Y, W, H);
	statusbox->resize(X + (192 * scale), Y + (10 * scale), 20 * scale, 40 * scale);
	avbox->resize(X + (10 * scale), Y + (10 * scale), 39 * scale , 39 * scale);
	name->resize(X + (62 * scale), Y + (19 * scale), 128 * scale, 11 * scale);
	status->resize(X + (62 * scale), Y + (31 * scale), 128 * scale, 11 * scale);
}

void namechangecb(Fl_Widget *w, long which)
{
	/*Fl_Input *f =(Fl_Input*)w;
	Event_t *e =Ev_new();

	e->T =Comm_ChangeName;

	if (which) Dictionary_set(APP->Config, "Tox.Name", f->value());
	else Dictionary_set(APP->Config, "Tox.Status", f->value());

	List_add(APP->Comm->WorkQueue, (void*)e);
	CommWork();*/
}
