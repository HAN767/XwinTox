#ifndef STATBOX_H_
#define STATBOX_H_

#include <FL/Fl_Box.H>

#include "Module/Module.h"

class StatusBox: public Fl_Box
{
public:
	// Ctor
	StatusBox(const XWF_hObj_t* hObj, int S);

	void draw();

	int scale;
	int status; /* 1 is green, 2 is red, 3 is yellow, 4 is grey */

	const XWF_hObj_t* hObj_;
};

#endif
