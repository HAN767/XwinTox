#include <vector>

extern "C"
{
#include "misc.h"
}
#include "contacts.h"
#include "xwintox_win.h"

ContactList_t *contactlist;

void SendMessagePressed(Fl_Widget* B , void*);

void ContactListGUIUpdate()
{
	int selected =-1;

	if(XwinTox->sidebar->contacts->entries.size() > 0)
	{
		if (XwinTox->sidebar->contacts->selected >= 0)
		{ selected = XwinTox->sidebar->contacts->selected; }
		XwinTox->sidebar->contacts->clear_all();
	}

	int YM =0;
	for (const auto contact : contactlist->contacts)
	{
		printf("Contact: %s\n", contact->name);
		ContactsEntry *newgui =new ContactsEntry(XwinTox->sidebar->contacts->x(), 
											  XwinTox->sidebar->contacts->y() + YM, 
											  XwinTox->sidebar->contacts->scale, 
											  contact);
		XwinTox->sidebar->contacts->add(newgui);
		XwinTox->sidebar->contacts->entries.push_back(newgui);
		YM += (50 * XwinTox->scale);

		GMessageArea *newarea =new GMessageArea(XwinTox->sidebar->scale, contact);
		newarea->hide();
		newarea->send->callback(&SendMessagePressed);

		XwinTox->add(newarea);
		XwinTox->contents->messageareas.push_back(newarea);

		if(contact->num == selected)
		{
			newgui->selected =1;
			XwinTox->contents->NewCurrentArea(FindContactMArea(contact));
			XwinTox->sidebar->contacts->selected =selected;
		}
	}
	XwinTox->redraw(); XwinTox->sidebar->contacts->redraw();
}

ContactsEntry *FindContactEntry(unsigned int num)
{
	for (const auto entry : XwinTox->sidebar->contacts->entries)
	{
		if(entry->contact->num == num) return entry;
	}
	dbg("Fail"); return 0;
}

GMessageArea *FindContactMArea(Contact_t *contact)
{
	for (const auto messagearea : XwinTox->contents->messageareas)
	{
		if(messagearea->contact == contact) return messagearea;
	}
	dbg("Fail"); return 0;
}

GMessageArea *FindContactMArea(unsigned int num)
{
	for (const auto messagearea : XwinTox->contents->messageareas)
	{
		if(messagearea->contact->num == num) return messagearea;
	}
	dbg("Fail"); return 0;
}

Contact_t *FindContact(unsigned int id)
{
	for (const auto contact : contactlist->contacts)
	{
		if(contact->num == id) return contact;
	}
	dbg("Fail"); return 0;
}
