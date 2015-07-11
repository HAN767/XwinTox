#ifndef GUI_H_
#define GUI_H_

#include <FL/Fl_Double_Window.H>
#include <FL/Fl_Menu_Bar.H>
#include <FL/Fl_Box.H>

#include "control/sidebar.h"
#include "control/gaddfrnd.h"
/*#include "control/gtrnsfer.h"*/

class XWContents: public Fl_Box
{
public:
	// Ctor
	XWContents(int X, int Y, int W, int H, int S);

	void draw();

	void NewCurrentArea(Fl_Group* G);

	Fl_Box *fake;
	GAddFriend *addfriend;
	/*GTransfers *transfers;

	vector <GMessageArea*> messageareas;*/

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

	void resize(int X, int Y, int W, int H);

	Fl_Menu_Bar *mbar;
	Sidebar *sidebar;
	XWContents *contents;
	//GAddFriend *addfriend;

	int scale, sblength, basex, basey;
};

//extern class XwinTox *Xw;

#endif
