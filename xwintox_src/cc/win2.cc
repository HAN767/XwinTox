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
	char *testmsg =(char*)calloc(1, sizeof(char)* 255);
	const char *id = ((GAddFriend*)B->parent())->id->value();
	const char *msg = ((GAddFriend*)B->parent())->message->value();
	// add a validity check here later //


	sprintf(testmsg, "sendfriendrequest %s %s", id, msg);
	List_add(&APP->Comm->WorkQueue, (void*)testmsg);
	CommWork();
}

void InitGUICallbacks()
{
	Xwin->contents->addfriend->send->callback(&AddFriendPressed);
}