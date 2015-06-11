#include <vector>

#include "contacts.h"
#include "xwintox_win.h"

ContactList_t *contactlist;

void ContactListGUIUpdate()
{
	Xwin->sidebar->contacts->clear_all();

	int YM =0;
	for (const auto contact : contactlist->contacts)
	{
		printf("Contact: %s\n", contact->name);
		ContactsEntry *newgui =new ContactsEntry(Xwin->sidebar->contacts->x(), 
											  Xwin->sidebar->contacts->y() + YM, 
											  Xwin->sidebar->contacts->scale, 
											  contact->name, contact->statusm);
		Xwin->sidebar->contacts->add(newgui);
		Xwin->sidebar->contacts->entries.push_back(newgui);
		YM += (50 * Xwin->scale);
	}
	Xwin->redraw(); Xwin->sidebar->contacts->redraw();
}
