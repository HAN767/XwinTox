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

void CommWork()
{
	mtx_lock (&APP->Comm->WorkMtx);
	APP->Comm->Work =1;
	cnd_broadcast(&APP->Comm->WorkCnd);
	mtx_unlock (&APP->Comm->WorkMtx);
	usleep(250);
	//APP->Comm->Work =0;
}

extern "C" int CXXMain()
{
	int scale =1;
	Contact_t *c;
	contactlist =(ContactList_t*)calloc(1, sizeof(ContactList_t));
	/*sleep(5);

	APP->Comm->WantQuit=1;
	CommWork();*/
	//Contact_t test, test2, test3;
	//test.name ="SylvieLorxu"; test.statusm= "Toxing on GNU/SylvieTox";
	/*strcpy(test2.name, "Sean Koreshi"); strcpy(test2.statusm, "Toxing in Islamabad");
	strcpy(test3.name, "Hexafluoride"); strcpy(test3.statusm, "Toxing in Istanbul");*/
	//contactlist->contacts.push_back(&test);	//contactlist->contacts.push_back(&test2); 
	//contactlist->contacts.push_back(&test3);
	while ((c =(Contact_t*)List_retrieve_and_remove_first(&APP->Xwin->ICQueue)) != 0)
	{
		contactlist->contacts.push_back(c);
	}
	//List_retrieve_and_remove_first(&APP->Xwin->ICQueue);
	//contactlist->contacts.push_back((Contact_t *)List_retrieve_and_remove_first(&APP->Xwin->ICQueue));
	Fl::visual(FL_RGB);
	XwinTox = new class XwinTox(640 * scale, 480 * scale, "XwinTox", scale);
	XwinTox->show();
	ContactListGUIUpdate();
	InitGUICallbacks();

	Fl::run();

	return 0;
}
