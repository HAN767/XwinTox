#ifndef CNTCTLST_H_
#define CNTCTLST_H_

#include <FL/Fl_Button.H>
#include <FL/Fl_Scroll.H>

#include "control/cntctent.h"

class ContactsList: public Fl_Scroll
{
public:
	// Ctor
	ContactsList(int X, int Y, int W, int H, int S);

	void draw();
	void resize(int X, int Y, int W, int H);
	int handle(int event);

	void clear_all();
	void deselect_all();

	vector <ContactsEntry*> entries;

	/* the purpose of 'startpoint' is to indicate where
	 * the first ContactEntry should be drawn relative to
	 * the y() of the contacts list*/
	int startpoint, selected, seltype, scale;
};

#endif
