#ifndef TRANSENT_H_
#define TRANSENT_H_

#include <FL/Fl_Group.H>

#include "xwintox.h"


class TransfersEntry: public Fl_Group
{
public:
	// Ctor
	TransfersEntry(int X, int Y, int S, Contact_t *C, int CO);

	void draw();

	Contact_t *contact;
	int scale, col;
};

#endif
