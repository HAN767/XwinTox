#ifndef GTRNSFER_H_
#define GTRNSFER_H_

#include <FL/Fl_Input.H>
#include <FL/Fl_Button.H>
#include <FL/Fl_Group.H>

#include "control/garea.h"

class GTransfers: public GArea
{
public:
	// Ctor
	GTransfers(int S);

	void resize(int X, int Y, int W, int H);
	void draw();
};

#endif
