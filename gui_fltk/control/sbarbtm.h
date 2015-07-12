#ifndef SBARBTM_H_
#define SBARBTM_H_

#include <FL/Fl_Group.H>

#include "xwintox.h"

#include "control/svgbox.h"

class Sidebar_Bottom_Area: public Fl_Group
{
public:
	// Ctor
	Sidebar_Bottom_Area(const XWF_hObj_t* hObj, int S);

	void resize(int X, int Y, int W, int H);

	void deselect_all();

	SVGBox *addfriend;
	SVGBox *newgroup;
	SVGBox *transfers;
	SVGBox *settings;

	int scale;

private:
	const XWF_hObj_t* hObj_;
};


#endif
