#include <string.h>
#include <FL/Fl_Group.H>
#include <FL/Fl_Input.H>

#include "xwin/svgs.h"

#include "control/gui.h"
#include "control/sbartop.h"
#include "control/svgbox.h"
#include "control/statbox.h"

void namechangecb(Fl_Widget *w, long which);

Sidebar_Top_Area::Sidebar_Top_Area(int S) : Fl_Group(Xw->basex * S,
	        Xw->basey * S, (Xw->sblength * S), 60 * S)
{
	scale =S;

	box(FL_FLAT_BOX);
	color(fl_rgb_color(28, 28, 28));

	statusbox =new StatusBox(x() + (192 * S), y() + (10 * S), 20 * S, 40 * S, S);
	avbox =new SVGBox(x() + (10 * S), y() + (10 * S), 40 * S , 40 * S, S,
	                  default_av, 0.3);
	name =new Fl_Input(x() + (62 * S), y() + (18 * S), 128 *S, 11 * S);
	status =new Fl_Input(x() + (62 * S), y() + (30 * S), 128 *S, 11 * S);
	name->textsize(12 * S);
	status->textsize(10 * S);
	name->box(FL_FLAT_BOX);
	name->color(fl_rgb_color(28, 28, 28));
	status->box(FL_FLAT_BOX);
	status->color(fl_rgb_color(28, 28, 28));
	name->textcolor(255);
	status->textcolor(54);
	name->value(Dictionary_get(APP->Config, "Tox.Name"));
	status->value(Dictionary_get(APP->Config, "Tox.Status"));
	name->when(FL_WHEN_RELEASE | FL_WHEN_ENTER_KEY);
	status->when(FL_WHEN_RELEASE | FL_WHEN_ENTER_KEY);
	name->callback(namechangecb, 1);
	status->callback(namechangecb);

	avbox->show();

	end();
}

void namechangecb(Fl_Widget *w, long which)
{
	Fl_Input *f =(Fl_Input*)w;
	Event_t *e =Ev_new();
	
	e->T =Comm_ChangeName;

	if (which) Dictionary_set(APP->Config, "Tox.Name", f->value());
	else Dictionary_set(APP->Config, "Tox.Status", f->value());
	
	List_add(APP->Comm->WorkQueue, (void*)e);
	CommWork();
}
