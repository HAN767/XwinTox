#ifndef GTRNSFER_H_
#define GTRNSFER_H_

#include <time.h>

#include <FL/Fl_Input.H>
#include <FL/Fl_Button.H>
#include <FL/Fl_Group.H>

#include "control/garea.h"
#include "control/translst.h"



class TransfersList;

class GTransfers: public GArea
{
public:
	// Ctor
	GTransfers(const XWF_hObj_t* hObj, int S);

	void resize(int X, int Y, int W, int H);
	void draw();

	TransfersList *list;
};

#endif
