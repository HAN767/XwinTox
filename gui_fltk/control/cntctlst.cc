#include <FL/Fl.H>
#include <FL/Fl_Scroll.H>

#include "control/xwin.h"
#include "control/cntctlst.h"
#include "control/svgbox.h"

#include "util.h"
#include "misc.h"
#include "list.h"

void updatecontacts(ContactsList *self, List_t *lstContacts)
{
	int iSel =-1;
	int iSeltype =-1;
	unsigned int wCurY;
	XwinTox *Xw;

	Fl_Widget *p =self;
    while (p->parent()) p =p->parent();
	Xw =(XwinTox*)p;

	if(self->selected >= 0)
	{
		iSel =self->selected;
		iSeltype =self->seltype;
	}

	self->clear_all();

	LIST_ITERATE_OPEN(lstContacts)
		XWContact_t *ctEntry =(XWContact_t*) e_data;
		dbg("Contact GUI. CTEntry num: %d \n", ctEntry->wNum);
		ContactsEntry *newgui =new ContactsEntry(self->hObj_, self->x(),
			self->y() + wCurY,
			self->scale,
			ctEntry, 0, 0);

		if(!FindContactMArea(Xw, ctEntry))
		{
			GMessageArea *newarea =new 
						GMessageArea(self->hObj_, self->scale,  ctEntry, 0, 0);
			newarea->hide();

			Xw->add(newarea);
			Xw->contents->messageareas.push_back(newarea);
		}

		self->add(newgui);
		self->entries.push_back(newgui);
		wCurY += (50 * self->scale);
	LIST_ITERATE_CLOSE(lstContacts)

}

void CtList_recv(int iType, PBMessage_t* msg, void* custom)
{
	ContactsList *self =(ContactsList*) custom;
	if (iType == clContacts)
	{
		Fl::lock();
		updatecontacts(self, (List_t*)msg->V);
		Fl::unlock();
	}
}

ContactsList::ContactsList(const XWF_hObj_t *hObj, int S)
	: Fl_Scroll(0, 0, 1, 1)
{
	hObj_ =hObj;
	scale =S;
	selected =-1;
	color(fl_rgb_color(65, 65, 65));
	startpoint =0 * S;

	Fl_Widget *p =this;
    while (p->parent()) p =p->parent();
	Xw =(XwinTox*)p;

	type(VERTICAL);
	end();

	hObj->pSvcs->fnSubscribe(hObj, clContacts, this, CtList_recv);
}

void ContactsList::draw()
{
	Fl_Scroll::draw();

	for(const ContactsEntry* entry : entries)
	{
		entry->icon->redraw();
	}
}

void ContactsList::resize(int X, int Y, int W, int H)
{
	Fl_Scroll::resize(X, Y, W, H);

	for(const auto entry : entries)
	{
		//entry->icon->draw();
	}
}

int ContactsList::handle(int event)
{
	switch(event)
	{
	case FL_PUSH:
		if(Fl::event_button() == FL_LEFT_MOUSE)
		{
			deselect_all();
			for(const auto entry : entries)
			{
				entry->redraw();
				entry->icon->redraw();
			}
		}
	}

	return Fl_Scroll::handle(event);
}

void ContactsList::clear_all()
{
	this->clear();
	for(const auto entry : entries)
	{
		Fl::delete_widget(entry);
	}
	entries.clear();
	this->redraw();
	parent()->redraw();
}

void ContactsList::deselect_all()
{
	for(const auto entry : entries)
	{
		entry->selected =0;
		entry->icon->show();
		entry->invicon->hide();
		entry->redraw();
		entry->icon->redraw();
	}
}
