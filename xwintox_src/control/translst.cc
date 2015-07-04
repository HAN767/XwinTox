#include <FL/Fl.H>
#include <FL/Fl_Scroll.H>

#include "control/translst.h"
#include "control/transent.h"

TransfersList::TransfersList(int X, int Y, int W, int H, int S)
	: Fl_Scroll(X, Y, W, H)
{
	scale =S;
	startpoint =0 * S;
	
	type(VERTICAL_ALWAYS);
	end();
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

	for(const auto entry : entries)
	{
		entry->resize(entry->x(), entry->y(), W, entry->h());
		entry->redraw();
	}
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
			return 1;
		}
	}

	return Fl_Scroll::handle(event);
}

void TransfersList::clear_all()
{
	this->clear();
	entries.clear();
	this->redraw();
	parent()->redraw();
}
