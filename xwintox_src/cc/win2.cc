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
	const char *id = ((GAddFriend*)B->parent())->id->value();
	const char *msg = ((GAddFriend*)B->parent())->message->value();
	// add a validity check here later //

	sprintf(amsg, "sendfriendrequest %s %s", id, msg);
	strcpy(bmsg, "savedata");

	List_add(&APP->Comm->WorkQueue, (void*)amsg);
	List_add(&APP->Comm->WorkQueue, (void*)bmsg);
	CommWork();
}

void InitGUICallbacks()
{
	Xwin->contents->addfriend->send->callback(&AddFriendPressed);
}
