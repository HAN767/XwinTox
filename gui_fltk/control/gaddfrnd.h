#ifndef GADDFRND_H_
#define GADDFRND_H_

#include <FL/Fl_Input.H>
#include <FL/Fl_Button.H>
#include <FL/Fl_Group.H>

#include "control/garea.h"

class GAddFriend: public GArea
{
public:
	// Ctor
	GAddFriend(int S);

	void resize(int X, int Y, int W, int H);
	void draw();

	Fl_Input *id;
	Fl_Input *message;
	Fl_Button *send;

	Fl_Group *fakebox;
};

#endif
