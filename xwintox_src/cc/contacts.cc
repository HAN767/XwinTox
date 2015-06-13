#include <vector>

#include "contacts.h"
#include "xwintox_win.h"

ContactList_t *contactlist;

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
	}
	XwinTox->redraw(); XwinTox->sidebar->contacts->redraw();
}
