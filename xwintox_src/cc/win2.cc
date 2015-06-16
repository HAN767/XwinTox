#include <stdio.h>

#include "xwintox_win.h"

extern "C"
{
#include "misc.h"
#include "list.h"

#include "xwintox.h"
}

void AddFriendPressed(Fl_Widget* B , void*)
{
	char *amsg =(char*)calloc(255, sizeof(char));
	char *bmsg =(char*)calloc(9, sizeof(char));
	char *cmsg =(char*)calloc(14, sizeof(char));
	const char *id = ((GAddFriend*)B->parent())->id->value();
	const char *msg = ((GAddFriend*)B->parent())->message->value();
	// add a validity check here later //

	sprintf(amsg, "sendfriendrequest %s %s", id, msg);
	strcpy(bmsg, "savedata");
	strcpy(cmsg, "getfriendlist");//getfriendlist");

	List_add(&APP->Comm->WorkQueue, (void*)amsg);
	List_add(&APP->Comm->WorkQueue, (void*)bmsg);
	List_add(&APP->Comm->WorkQueue, (void*)cmsg);
	CommWork();
}

void InitGUICallbacks()
{
	XwinTox->contents->addfriend->send->callback(&AddFriendPressed);
}

char *GetDisplayName(Contact_t *contact, size_t LenLimit)
{
	static char name[255] = { 0 };

	if (strlen(contact->name) == 0)
	{ 
		strncpy(name, contact->pubkey, LenLimit-3);
		name[LenLimit-2] ='.'; name[LenLimit-1] ='.'; name[LenLimit] ='.';

	}
	else if (strlen(contact->name) >= LenLimit)
	{
		strncpy(name, contact->name, LenLimit-3);
		name[LenLimit-2] ='.'; name[LenLimit-1] ='.'; name[LenLimit] ='.';
	}
	else
	{
		strcpy(name, contact->name);
	}

	return name;
}

char *GetDisplayStatus(Contact_t *contact, size_t LenLimit)
{
	static char status[255] = { 0 };

	if (strlen(contact->statusm) >= LenLimit)
	{
		strncpy(status, contact->statusm, LenLimit - 3);
		status[LenLimit - 2] ='.'; status[LenLimit - 1] ='.'; 
		status[LenLimit] ='.';
	}
	else if (strlen (contact->statusm) > 0)
	{
		strcpy(status, contact->statusm);
	}
	else if (strlen (contact->statusm) == 0)
	{
		strcpy(status, "Unknown");
	}

	return status;
}


