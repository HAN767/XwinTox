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
	XwinTox->sidebar->contacts->clear_all();

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
	}
	XwinTox->redraw(); XwinTox->sidebar->contacts->redraw();
}

GMessageArea *FindContactMArea(Contact_t *contact)
{
	for (const auto messagearea : XwinTox->contents->messageareas)
	{
		if(messagearea->contact == contact) return messagearea;
	}
	dbg("Fail"); return 0;
}

GMessageArea *FindContactMArea(unsigned int id)
{
	for (const auto messagearea : XwinTox->contents->messageareas)
	{
		if(messagearea->contact->num == id) return messagearea;
	}
	dbg("Fail"); return 0;
}
