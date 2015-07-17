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

char *GetDisplayName(XWContact_t *contact, size_t LenLimit)
{
	static char name[255] = { 0 };

	if(contact->pszName == 0 || strlen(contact->pszName) == 0)
	{
		strncpy(name, contact->pszID, LenLimit);
		name[LenLimit-2] ='.';
		name[LenLimit-1] ='.';
		name[LenLimit] ='.';
		name[LenLimit+1] = '\0';

	}
	else if(strlen(contact->pszName) >= LenLimit)
	{
		strncpy(name, contact->pszName, LenLimit);
		name[LenLimit-2] ='.';
		name[LenLimit-1] ='.';
		name[LenLimit] ='.';
		name[LenLimit+1] = '\0';
	}
	else
	{
		strcpy(name, contact->pszName);
	}

	return name;
}

char *GetDisplayStatus(XWContact_t *contact, size_t LenLimit)
{
	static char status[255] = { 0 };

	if(contact->pszStatus == 0 || strlen(contact->pszStatus) == 0)
	{
		strcpy(status, "Unknown");
	}
	else if (strlen(contact->pszStatus) >= LenLimit)
	{
		strcpy(status, GetShortenedText(contact->pszStatus, LenLimit));
	}
	else if(strlen(contact->pszStatus) > 0)
	{
		strcpy(status, contact->pszStatus);
	}

	return status;
}

char *GetShortenedText(char* text, size_t LenLimit)
{
	static char name[255] = { 0 };

	strncpy(name, text, LenLimit);
	name[LenLimit-2] ='.';
	name[LenLimit-1] ='.';
	name[LenLimit] ='.';
	name[LenLimit+1] = '\0';
	return name;
}

void RemoveSpaces(char * s)
{
	char *p =s;
	int l =strlen(p);

	while(isspace(p[l - 1])) p[--l] =0;

	while(*p && isspace(*p)) ++p, --l;

	memmove(s, p, l + 1);
}
