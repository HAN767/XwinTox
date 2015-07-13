#include "util.h"

ContactsEntry *FindContactEntry(XwinTox *Xw, unsigned int num)
{
	for(const auto entry : Xw->sidebar->contacts->entries)
	{
		if(entry->contact->wNum == num) return entry;
	}

	return 0;
}

GMessageArea *FindContactMArea(XwinTox *Xw, XWContact_t *contact)
{
	for(const auto messagearea : Xw->contents->messageareas)
	{
		if(messagearea->contact == contact) return messagearea;
	}

	return 0;
}

GMessageArea *FindContactMArea(XwinTox *Xw, unsigned int num)
{
	for(const auto messagearea : Xw->contents->messageareas)
	{
		if(messagearea->contact->wNum == num) return messagearea;
	}

	return 0;
}

/*XWContact_t *FindContact(XwinTox *Xw, unsigned int id)
{
	for(const auto contact : Xw->sidebar->contactscontactlist->contacts)
	{
		if(contact->wNum == id) return contact;
	}

	return 0;
}*/
