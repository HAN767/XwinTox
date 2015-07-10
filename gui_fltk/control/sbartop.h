#ifndef SBARTOP_H_
#define SBARTOP_H_

#include <FL/Fl_Group.H>
#include <FL/Fl_Input.H>

#include "control/statbox.h"

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

#endif
