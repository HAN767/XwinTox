#include <stdio.h>

#include "xwintox_win.h"
#include "contacts.h"

extern "C"
{
#include "toxaemia_rpc.h"
#include "misc.h"
#include "list.h"

#include "xwintox.h"
}

void AddFriendPressed(Fl_Widget* B , void*)
{
	XwinTox->contents->NewCurrentArea(XwinTox->contents->addfriend);
	B->box(FL_FLAT_BOX);
	B->color(fl_rgb_color(68, 68, 67));
	XwinTox->sidebar->contacts->deselect_all();
}

void NewGroupchatPressed(Fl_Widget *B, void*)
{
	char *amsg =(char*)calloc(13, sizeof(char));
	strcpy(amsg, "newgroupchat");
	dbg("New groupchat\n");
	List_add(&APP->Comm->WorkQueue, (void*)amsg);
}

void AddFriendSendPressed(Fl_Widget* B , void*)
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

void SendMessagePressed(Fl_Widget* B , void*)
{
	char *amsg =(char*)calloc(1025, sizeof(char));
	static char nmsg[1024];
	unsigned int id;
	const char *msg =((GMessageArea*)B->parent())->message->value();

	// add a validity check here later //

	sprintf(nmsg, "%s: %s\n", XwinTox->sidebar->top_area->name->value(),
	        ((GMessageArea*)B->parent())->message->value());

	if(!((GMessageArea*)B->parent())->mtype) /* contact */
	{
		id =((GMessageArea*)B->parent())->contact->num;
		sprintf(amsg, "sendmessage %d %d %s", MFRIEND, id, msg);
		FindContactMArea(id)->moutbuffer->append(nmsg);
		FindContactMArea(id)->message->value("");
		FindContactMArea(id)->moutput->scroll
		(FindContactMArea(id)->moutput->count_lines(0,
		        FindContactMArea(id)->moutbuffer->length(), 1), 0);
	}
	else
	{
		id =((GMessageArea*)B->parent())->groupchat->num;
		sprintf(amsg, "sendmessage %d %d %s", MGCHAT, id, msg);
		FindGroupchatMArea(id)->message->value("");
		//FindGroupchatMArea(id)->moutbuffer->append(nmsg);
	}

	List_add(&APP->Comm->WorkQueue, (void*)amsg);
	CommWork();
}

void WindowClosed(Fl_Widget *widget, void *)
{
	char *amsg =(char*)calloc(5, sizeof(char));
	Fl_Double_Window *window = (Fl_Double_Window *)widget;

	sprintf(amsg, "quit");

	List_add(&APP->Comm->WorkQueue, (void*)amsg);
	CommWork();
}

void InitGUICallbacks()
{
	XwinTox->contents->addfriend->send->callback(&AddFriendSendPressed);
	XwinTox->sidebar->bottom_area->addfriend->callback(&AddFriendPressed);
	XwinTox->sidebar->bottom_area->newgroup->callback(&NewGroupchatPressed);
}

char *GetDisplayName(Contact_t *contact, size_t LenLimit)
{
	static char name[255] = { 0 };

	if(strlen(contact->name) == 0)
	{
		strncpy(name, contact->pubkey, LenLimit);
		name[LenLimit-2] ='.';
		name[LenLimit-1] ='.';
		name[LenLimit] ='.';
		name[LenLimit+1] = '\0';

	}
	else if(strlen(contact->name) >= LenLimit)
	{
		strncpy(name, contact->name, LenLimit);
		name[LenLimit-2] ='.';
		name[LenLimit-1] ='.';
		name[LenLimit] ='.';
		name[LenLimit+1] = '\0';
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

	if(strlen(contact->statusm) >= LenLimit)
	{
		strncpy(status, contact->statusm, LenLimit);
		status[LenLimit - 2] ='.';
		status[LenLimit - 1] ='.';
		status[LenLimit] ='.';
		status[LenLimit+1] = '\0';
	}
	else if(strlen(contact->statusm) > 0)
	{
		strcpy(status, contact->statusm);
	}
	else if(strlen(contact->statusm) == 0)
	{
		strcpy(status, "Unknown");
	}

	return status;
}

void AddLine(ToxMessageType type, unsigned int id, unsigned int pid, char* msg)
{
	time_t rawtime;
	struct tm *info;
	static char nmsg[1024];
	static char date[256];

	time(&rawtime);
	info = localtime(&rawtime);
	strftime(date, 255, "%H:%M", info);

	if(type == MFRIEND)
	{
		Contact_t *contact =FindContact(id);
		sprintf(nmsg, "%s: %s\n", contact->name, msg);
		FindContactMArea(id)->moutbuffer->append(nmsg);
		FindContactMArea(id)->moutput->scroll
		(FindContactMArea(id)->moutput->count_lines(0,
		        FindContactMArea(id)->moutbuffer->length(), 1), 0);
	}
	else if(type == MGCHAT)
	{
		Groupchat_t *gchat =FindGroupchat(id);
		sprintf(nmsg, "%s: %s\n", GroupchatGetPeerName(id, pid), msg);
		FindGroupchatMArea(id)->moutbuffer->append(nmsg);
		FindGroupchatMArea(id)->moutput->scroll
		(FindGroupchatMArea(id)->moutput->count_lines(0,
		        FindGroupchatMArea(id)->moutbuffer->length(), 1), 0);
	}

}
