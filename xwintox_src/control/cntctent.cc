#include <algorithm>
#include <vector>
#include <FL/Fl_Box.H>

#include "cc/svgs.h"
#include "cc/contacts.h"

#include "control/gui.h"
#include "control/msgarea.h"
#include "control/cntctlst.h"
#include "control/sidebar.h"
#include "control/svgbox.h"

extern int CGUIUPDFLAG;

void ce_deletecontact(ContactsEntry *ce)
{
	vector <ContactsEntry*> *ref =&
		((ContactsList*) ce->parent())->entries;
	ref->erase(std::remove(ref->begin(), ref->end(), ce),
	ref->end());
	ce->parent()->remove(ce);
	Fl::delete_widget(ce);

	if (ce->type == 0)
	{
		DeleteContact(ce->contact->num);
		Fl::delete_widget(FindContactMArea(ce->contact));
	}
	else
	{
	Groupchat_t *todel;
	GMessageArea *mtodel;
	char *amsg =(char*)calloc(255, sizeof(char));

	sprintf(amsg, "leavegroupchat %d", ce->groupchat->num);
	List_add(&APP->Comm->WorkQueue, (void*)amsg);

	todel =ce->groupchat;
	vector <Groupchat_t*> *ref=&groupchats;
	vector <GMessageArea*> *mref =&Xw->contents->messageareas;
	free(todel->name);
	free(todel->peers);
	free(todel->peers_raw);
	free(todel->peers_raw_lens);
	free(todel);
	ref->erase(std::remove(ref->begin(), ref->end(), todel), ref->end());

	for(const auto messagearea : Xw->contents->messageareas)
	{
		if(messagearea->groupchat == todel) mtodel =messagearea;
	}

	mref->erase(std::remove(mref->begin(), mref->end(), mtodel),
				mref->end());

	if(Xw->contents->currentarea == FindGroupchatMArea(todel))
	{
		Xw->contents->currentarea =Xw->contents->addfriend;
	}

	CommWork();
	}
	CGUIUPDFLAG =1;
}

ContactsEntry::ContactsEntry(int X, int Y, int S, Contact_t *C, Groupchat_t *G,
                             short T)
	: Fl_Box(X, Y, 224 * S, 50 * S)
{
	scale =S;
	contact =C;
	groupchat =G;
	type =T;
	selected =0;

	if(!T)
	{
		icon =new SVGBox(X+ (4 * S), Y+ (2 * S), 46 * S, 46 *S, S,
		                 default_av, 0.35);
		invicon =new SVGBox(X+ (4 * S), Y+ (2 * S), 46 * S, 46 *S, S,
		                    default_av, 0.35);
		invicon->hide();
		groupchat =new Groupchat_t;
		groupchat->num =65535;
	}
	else
	{
		icon =new SVGBox(X- (8 * S), Y+ (2 * S), 46 * S, 46 *S, S,
		                 groupsvg, 0.63);
		invicon =new SVGBox(X+ (14 * S), Y+ (12 * S), 46 * S, 46 *S, S,
		                    groupsvg2, 0.63);
		invicon->hide();
		contact =new Contact_t;
		contact->num =65535;
	}

	box(FL_FLAT_BOX);
	color(fl_rgb_color(65, 65, 65));
}

void ContactsEntry::draw()
{
	int txt_color =255;
	char *name, *status;

	if(selected)
	{
		color(255);
		txt_color =0;
		icon->hide();
		invicon->show();
	}
	else
	{
		color(fl_rgb_color(65, 65, 65));
		txt_color=255;
		invicon->hide();
		icon->show();
	}

	Fl_Box::draw();

	if(!type) /* todo: resize() calls to the svgbox, make it work */
	{
		name =GetDisplayName(contact, 16);
		status =GetDisplayStatus(contact, 25);

		if(contact->connected) fl_color(2);
		else fl_color(FL_RED);

		fl_pie(x() + (185 * scale), this->y() + (20 * scale), 10 * scale,
		       10 * scale, 0, 360);
	}
	else
	{
		name =groupchat->name;
		status ="";
	}

	fl_color(txt_color);
	fl_font(FL_HELVETICA, 12 * scale);
	fl_draw(name, x() + (50 * scale), y() + (22 * scale));
	fl_font(FL_HELVETICA, 10 * scale);
	fl_draw(status, x() + (50 * scale), y() + (36 * scale));
	icon->draw(); /* make it account for scrollbar::value(); */
	invicon->draw();
}

int ContactsEntry::handle(int event)
{
	switch(event)
	{
	case FL_PUSH:
		if(Fl::event_button() == FL_LEFT_MOUSE)
		{
			for(const auto entry : ((ContactsList*) parent())->entries)
			{
				entry->selected =0;
				entry->redraw();
				entry->icon->redraw();
				entry->invicon->redraw();
				((Sidebar*)(parent()->parent()))->bottom_area->deselect_all();
			}

			selected =1;
			((ContactsList*) parent())->seltype =type;
			redraw();
			icon->redraw();
			invicon->redraw();

			if(!type)
			{
				((ContactsList*) parent())->selected =contact->num;
			}
			else
			{
				((ContactsList*) parent())->selected =groupchat->num;
			}

			icon->hide();
			invicon->show();

			if(!type)
			{
				Xw->contents->NewCurrentArea(FindContactMArea(contact));
			}
			else
			{
				Xw->contents->NewCurrentArea(FindGroupchatMArea(groupchat));
			}

			return 1;
		}
		else if(Fl::event_button() == FL_RIGHT_MOUSE)
		{
			Fl_Menu_Item contmenu[] = { { "Delete contact" }, { 0 } };
			contmenu->labelsize(12 * scale);
			const Fl_Menu_Item *m = contmenu->popup(Fl::event_x(), Fl::event_y(),
			                                        0, 0, 0);

			if(!m) return 0;
			else if(strcmp(m->label(), "Delete contact") == 0)
			{
				ce_deletecontact(this);
				return 0;
			}
		}
	}

	return 0;
}
