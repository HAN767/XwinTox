#ifndef TRANSENT_H_
#define TRANSENT_H_

#include <FL/Fl_Button.H>
#include <FL/Fl_Group.H>
#include <FL/Fl_Progress.H>

#include "xwintox.h"
#include "gtrnsfer.h"

typedef struct Transfer_s Transfer_t;

class TransfersEntry: public Fl_Group
{
public:
	// Ctor
	TransfersEntry(int X, int Y, int S, Transfer_t *T, int I);

	void draw();
	void resize(int X, int Y, int W, int H);

	Fl_Progress *progress;
	Fl_Button *accept, *reject;

	Transfer_t *transfer;
	int scale, inv;
};

#endif
