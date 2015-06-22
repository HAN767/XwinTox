#ifndef MSGAREA_H_
#define MSGAREA_H_

#include "cc/xwintox_win.h"

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

#endif