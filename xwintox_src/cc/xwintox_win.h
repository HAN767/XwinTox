#ifndef XWINTOX_WIN_H_
#define XWINTOX_WIN_H_

#include <vector>
using namespace std;

#include <FL/Fl.H>
#include <FL/Fl_Box.H>
#include <FL/Fl_Double_Window.H>
#include <FL/Fl_Input.H>
#include <FL/Fl_Multiline_Output.H>
#include <FL/Fl_Menu_Bar.H>
#include <FL/Fl_Text_Display.H>
#include <FL/Fl_RGB_Image.H>
#include <FL/Fl_Scroll.H>
#include <FL/Fl_Select_Browser.H>
#include <FL/Fl_Button.H>

#include "contacts.h"

class StatusBox: public Fl_Box
{
public:
	// Ctor
	StatusBox(int X, int Y, int W, int H, int S);

	void draw();

	int scale;
	int status; /* 1 is green, 2 is red, 3 is yellow, 4 is grey */
};

class SVGBox: public Fl_Box
{
public:
	// Ctor
	SVGBox(int X, int Y, int W, int H, int S, const char* pic, double factor);
	void draw();
	int handle(int event);
	void hide();
	void show();

	int scale;
	unsigned char *img_r;
	Fl_RGB_Image *img;
	Fl_Color bg;
};

class ContactsEntry: public Fl_Box
{
public:
	// Ctor
	ContactsEntry(int X, int Y, int S, Contact_t *C, Groupchat_t *G, short T);

	void draw();
	int handle(int event);

	SVGBox *icon; SVGBox *invicon;

	short type; /* 0 = contact, 1 = groupchat */
	Contact_t *contact;
	Groupchat_t *groupchat;
	int selected, scale;
};

class ContactsList: public Fl_Scroll
{
public:
	// Ctor
	ContactsList(int X, int Y, int W, int H, int S);

	void draw();
	void resize (int X, int Y, int W, int H);
	int handle(int event);

	void clear_all();
	void deselect_all();

	vector <ContactsEntry*> entries;

	int selected, seltype, scale;
};

class Sidebar_Top_Area: public Fl_Group
{
public:
	// Ctor
	Sidebar_Top_Area(int S);

	SVGBox *avbox;
	Fl_Input *name;
	Fl_Input *status;
	StatusBox *statusbox;


	int scale;
};

class Sidebar_Bottom_Area: public Fl_Group
{
public:
	// Ctor
	Sidebar_Bottom_Area(int S);

	void resize(int X, int Y, int W, int H);

	void deselect_all();

	SVGBox *addfriend;
	SVGBox *newgroup;
	SVGBox *transfers;
	SVGBox *settings;

	int scale;
};

class Sidebar: public Fl_Group
{
public:
	// Ctor
	Sidebar(int S);

	void resize (int X, int Y, int W, int H);

	Sidebar_Top_Area *top_area;
	ContactsList *contacts;
	Sidebar_Bottom_Area *bottom_area;

	int scale;
};

class GArea: public Fl_Group
{
public:
	// Ctor
	GArea(int S, const char *C);

	void draw();

	const char *caption;
	int scale;
};

class GAddFriend: public GArea
{
public:
	// Ctor
	GAddFriend(int S);

	void resize (int X, int Y, int W, int H);
	void draw();

	Fl_Input *id;
	Fl_Input *message;
	Fl_Button *send;

	Fl_Group *fakebox;
};

class GMessageArea: public Fl_Group
{
public:
	// Ctor
	GMessageArea(int S, Contact_t *C, Groupchat_t *G, short T);

	void resize(int X, int Y, int W, int H);
	void draw();

	SVGBox *icon;
	Fl_Multiline_Output *gnames;
	Fl_Select_Browser *names;
	Fl_Text_Display *moutput;
	Fl_Text_Buffer *moutbuffer;
	Fl_Input *message;
	Fl_Button *send;

	int scale;
	short mtype;
	Contact_t *contact;
	Groupchat_t *groupchat;
};

class XWContents: public Fl_Box
{
public:
	// Ctor
	XWContents(int S);

	void draw();

	void NewCurrentArea(Fl_Group* G);

	Fl_Box *fake;
	GAddFriend *addfriend;

	vector <GMessageArea*> messageareas;


	Fl_Group *currentarea;
	Fl_Group *newcurrentarea;
	int scale;
};

class XwinTox : public Fl_Double_Window
{
public:
	// Ctor
	XwinTox(int w, int h, const char* c, int S);
	void init2();

	void resize (int X, int Y, int W, int H);

	Fl_Menu_Bar *mbar;
	Sidebar *sidebar;
	XWContents *contents;
	GAddFriend *addfriend;

	int scale, sblength, basex, basey;
};

extern "C" XwinTox* XwinTox;

#endif
