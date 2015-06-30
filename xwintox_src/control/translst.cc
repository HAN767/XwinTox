#include <FL/Fl.H>
#include <FL/Fl_Scroll.H>

#include "control/translst.h"
#include "control/transent.h"

TransfersList::TransfersList(int X, int Y, int W, int H, int S)
	: Fl_Scroll(X, Y, W, H)
{
	scale =S;
	startpoint =0 * S;
	
	type(VERTICAL);
}

void TransfersList::draw()
{
	Fl_Scroll::draw();

	for(const auto entry : entries)
	{
		entry->redraw();
	}
}

void TransfersList::resize(int X, int Y, int W, int H)
{
	Fl_Scroll::resize(X, Y, W, H);
}

int TransfersList::handle(int event)
{
	switch(event)
	{
	case FL_PUSH:
		if(Fl::event_button() == FL_LEFT_MOUSE)
		{
			for(const auto entry : entries)
			{
				entry->redraw();
			}
		}
	}

	Fl_Scroll::handle(event);

	return 0;
}

void TransfersList::clear_all()
{
	this->clear();
	entries.clear();
	this->redraw();
	parent()->redraw();
}
