#include <threads.h>
#include <unistd.h>
#include <vector>
using namespace std;

#include <FL/Fl.H>

#include "control/gui.h"
#include "contacts.h"

extern "C"
{
#include "misc.h"
#include "dictionary.h"
#include "list.h"

#include "toxaemia_rpc.h"
#include "etc.h"
#include "xwintox.h"
}

class XwinTox* Xw;
extern "C" Xwin_t *Xwin;

int CGUIUPDFLAG =0;

void InitGUICallbacks();
void AddLine(ToxMessageType type, unsigned int id, unsigned int pid, char* msg);

void CommWork()
{
	mtx_lock(&APP->Comm->WorkMtx);
	APP->Comm->Work =1;
	mtx_unlock(&APP->Comm->WorkMtx);
	usleep(250);
	//APP->Comm->Work =0;
}

void SaveData()
{
	char *amsg =(char*)calloc(9, sizeof(char));
	strcpy(amsg, "savedata");
	List_add(&APP->Comm->WorkQueue, amsg);
}

void ProcessEvents()
{
	ToxEvent_t *Event;

	while((Event =(ToxEvent_t*)
	              List_retrieve_and_remove_first(&APP->Xwin->Events)) != 0)
	{
		dbg("Event: Type %d ParamID %d Param0 %d Param1 %s\n", Event->type,
		    Event->paramid, Event->param0, Event->param1);

		if(Event->type == FMESSAGE) AddLine(MFRIEND, Event->paramid, 0,
			                                    Event->param1);

		if(Event->type == GMESSAGE) AddLine(MGCHAT, Event->paramid,
			                                    Event->param0, Event->param1);
		else if(Event->type == FADDED && Event->param0 == 1 &&
		        Event->paramid != -1)
		{
			FriendRequestSuccess(Event->paramid);
		}
		else if(Event->type == FCONN)
		{
			FindContact(Event->paramid)->connected =Event->param0;
			FindContactEntry(Event->paramid)->redraw();
		}
		else if(Event->type == FNAME)
		{
			FindContact(Event->paramid)->name =strdup(Event->param1);
			FindContactEntry(Event->paramid)->redraw();
			SaveData();
		}
		else if(Event->type == FSTATUS)
		{
			FindContact(Event->paramid)->statusm =strdup(Event->param1);
			FindContactEntry(Event->paramid)->redraw();
			SaveData();
		}
		else if(Event->type == GNEW)
		{
			GroupchatCreateSuccess(Event->paramid);
		}
		else if(Event->type == GNAMES)
		{
			GroupchatNames(Event->paramid, Event->param3.param3_len,
			               Event->param1, Event->param2.param2_val,
			               Event->param3.param3_val, Event->param2.param2_len);
		}

		free(Event->param1);
		free(Event->param2.param2_val);
		free(Event->param3.param3_val);
		free(Event);
	}
}

extern "C" int CXXMain()
{
	int scale =1;
	Contact_t *c;
	contactlist =(ContactList_t*)calloc(1, sizeof(ContactList_t));

	while((c =(Contact_t*)List_retrieve_and_remove_first(&APP->Xwin->ICQueue))
	        != 0)
	{
		contactlist->contacts.push_back(c);
	}

	Fl::visual(FL_RGB);
	Xw = new class XwinTox(640 * scale, 480 * scale, "XwinTox", scale);
	Xw->init2();
	Xw->show();
	ContactListGUIUpdate();
	InitGUICallbacks();

	while(1)
	{
		Fl::wait(0.1);

		if(APP->Xwin->Events) ProcessEvents();

		if(CGUIUPDFLAG)
		{
			ContactListGUIUpdate();
			CGUIUPDFLAG =0;
		}
	}

	return 0;
}
