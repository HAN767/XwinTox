#ifndef SIDEBAR_H_
#define SIDEBAR_H_

#include <FL/Fl_Button.H>
#include <FL/Fl_Group.H>

#include "control/sbarbtm.h"
#include "control/sbartop.h"
#include "control/frequest.h"
#include "control/cntctlst.h"


class Sidebar: public Fl_Group
{
public:
	// Ctor
	Sidebar(int S);

	void resize(int X, int Y, int W, int H);

	Sidebar_Top_Area *top_area;
	/* top2*/
	Fl_Button *frbutton;
	FriendRequests *f_reqs;
	/* end top2 */
	ContactsList *contacts;
	Sidebar_Bottom_Area *bottom_area;

	int scale, top2_h;
};

#endif
