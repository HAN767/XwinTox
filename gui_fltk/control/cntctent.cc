#include <algorithm>
#include <vector>

#include <FL/Fl_Box.H>
#include <FL/Fl_Menu.H>

#include "nanosvg/svgs.h"
#include "xwintox.h"

#include "control/xwin.h"
#include "control/msgarea.h"
#include "control/cntctlst.h"
#include "control/sidebar.h"
#include "control/svgbox.h"
#include "util.h"

void ce_deletecontact(ContactsEntry *ce)
{
	/*vector <ContactsEntry*> *ref =&
	                              ((ContactsList*) ce->parent())->entries;
	ref->erase(std::remove(ref->begin(), ref->end(), ce),
	           ref->end());
	ce->parent()->remove(ce);
	Fl::delete_widget(ce);

	if(ce->type == 0)
	{
		DeleteContact(ce->contact->num);
		Fl::delete_widget(FindContactMArea(ce->contact));
	}
	else
	{
		Groupchat_t *todel;
		GMessageArea *mtodel =0;
		Event_t *e =Ev_new();

		e->T =Comm_LeaveGroupchat;
		e->ID =ce->groupchat->num;
		List_add(APP->Comm->WorkQueue, (void*)e);

		todel =ce->groupchat;
		vector <Groupchat_t*> *ref=&groupchats;
		vector <GMessageArea*> *mref =&Xw->contents->messageareas;

		free(todel->name);

		free(todel->peers);

		if(todel->peers_raw)
		{
			free(todel->peers_raw);
			free(todel->peers_raw_lens);
		}

		free(todel);
		ref->erase(std::remove(ref->begin(), ref->end(), todel), ref->end());

		for(const auto messagearea : Xw->contents->messageareas)
		{
			if(messagearea->groupchat == todel) mtodel =messagearea;
		}

		mref->erase(std::remove(mref->begin(), mref->end(), mtodel),
		            mref->end());

		if(Xw->contents->currentarea == mtodel)
		{
			Xw->contents->NewCurrentArea(Xw->contents->addfriend);
		}

		Fl::delete_widget(mtodel);

		CommWork();
	}

	CGUIUPDFLAG =1;*/
}

ContactsEntry::ContactsEntry(const XWF_hObj_t *hObj, int X, int Y, int S,
                             XWContact_t *C, XWGroupchat_t *G, short T)
	: Fl_Box(X, Y, 224 * S, 50 * S)
{
	hObj_ =hObj;
	scale =S;
	contact =C;
	groupchat =G;
	type =T;
	selected =0;

	if(!T)
	{
		icon =new SVGBox(X+ (4 * S), Y+ (2 * S), 40 * S, 40 *S, S,
		                 default_av, 1);
		invicon =new SVGBox(X+ (4 * S), Y+ (2 * S), 40 * S, 40 *S, S,
		                    default_av, 1);
		invicon->hide();
		groupchat =new XWGroupchat_t;
		groupchat->wNum =65535;
	}
	else
	{
		icon =new SVGBox(X- (8 * S), Y+ (2 * S), 40 * S, 40 *S, S,
		                 groupsvg, 0.63);
		invicon =new SVGBox(X+ (14 * S), Y+ (12 * S), 40 * S, 40 *S, S,
		                    groupsvg2, 0.63);
		invicon->hide();
		contact =new XWContact_t;
		contact->wNum =65535;
	}

	icon->box(FL_ENGRAVED_BOX);
	icon->color(fl_rgb_color(130, 128, 133));

	invicon->box(FL_ENGRAVED_BOX);
	invicon->color(fl_rgb_color(130, 128, 133));

	box(FL_FLAT_BOX);
	color(fl_rgb_color(65, 65, 65));
}

void ContactsEntry::draw()
{
	int txt_color =255;
	char *name, *status;

	fl_push_clip(x(), y(), w(), h());

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
		status =GetDisplayStatus(contact, 23);

		if(contact->wConnected) fl_color(2);
		else fl_color(FL_RED);

		fl_pie(x() + (185 * scale), this->y() + (20 * scale), 10 * scale,
		       10 * scale, 0, 360);
		icon->resize(x()+ (5 * scale), y()+ (5 * scale), 40 * scale, 40 *scale);
		invicon->resize(x()+ (5 * scale), y()+ (5 * scale), 40 * scale,
		                40 *scale);
	}
	else
	{
		name =groupchat->pszName;
		status =(char*)"";

		icon->resize(x()- (8 * scale), y()+ (2 * scale), 40 * scale, 40 * scale);
		invicon->resize(x()+ (14 * scale), y()+ (12 * scale), 40 * scale,
		                40 *scale);
	}

	fl_color(txt_color);
	fl_font(FL_HELVETICA, 12 * scale);
	fl_draw(name, x() + (54 * scale), y() + (22 * scale));
	fl_font(FL_HELVETICA, 10 * scale);
	fl_draw(status, x() + (54 * scale), y() + (36 * scale));
	icon->draw(); /* make it account for scrollbar::value(); */
	invicon->draw();

	fl_pop_clip();
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
				((ContactsList*) parent())->selected =contact->wNum;
			}
			else
			{
				((ContactsList*) parent())->selected =groupchat->wNum;
			}

			icon->hide();
			invicon->show();

			if(!type)
			{
				((ContactsList*) parent())->Xw->contents->NewCurrentArea
				(FindContactMArea(((ContactsList*) parent())->Xw, contact));
			}
			else
			{
				//Xw->contents->NewCurrentArea(Xw, FindGroupchatMArea(groupchat));
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
