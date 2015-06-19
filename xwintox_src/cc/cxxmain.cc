#include <threads.h>
#include <unistd.h>
#include <vector>
using namespace std;

#include <FL/Fl.H>

#include "xwintox_win.h"
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

class XwinTox* XwinTox;
extern "C" Xwin_t *Xwin;

void InitGUICallbacks();
void AddLine(unsigned int id, char* msg);

void CommWork()
{
	mtx_lock (&APP->Comm->WorkMtx);
	APP->Comm->Work =1;
	mtx_unlock (&APP->Comm->WorkMtx);
	usleep(250);
	//APP->Comm->Work =0;
}

void ProcessEvents()
{
	ToxEvent_t *Event;
	while ((Event =(ToxEvent_t*)
			List_retrieve_and_remove_first(&APP->Xwin->Events)) != 0)
	{
		dbg("Event: Type %d ParamID %d Param0 %d Param1 %s\n", Event->type,
			Event->paramid, Event->param0, Event->param1);
		if(Event->type == FMESSAGE) AddLine(Event->paramid, Event->param1);
		free(Event->param1); free(Event->param2); free(Event->param3);
		free (Event);
	}
}

extern "C" int CXXMain()
{
	int scale =1;
	Contact_t *c;
	contactlist =(ContactList_t*)calloc(1, sizeof(ContactList_t));

	while ((c =(Contact_t*)List_retrieve_and_remove_first(&APP->Xwin->ICQueue)) != 0)
	{
		contactlist->contacts.push_back(c);
	}

	Fl::visual(FL_RGB);
	XwinTox = new class XwinTox(640 * scale, 480 * scale, "XwinTox", scale);
	XwinTox->init2();
	XwinTox->show();
	ContactListGUIUpdate();
	InitGUICallbacks();

	while (1)
	{
		Fl::wait(0.1);
		if(APP->Xwin->Events) ProcessEvents();
	}

	return 0;
}
