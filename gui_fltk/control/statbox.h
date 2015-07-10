#ifndef STATBOX_H_
#define STATBOX_H_

#include <FL/Fl_Box.H>

class StatusBox: public Fl_Box
{
public:
	// Ctor
	StatusBox(int X, int Y, int W, int H, int S);

	void draw();

	int scale;
	int status; /* 1 is green, 2 is red, 3 is yellow, 4 is grey */
};

#endif
