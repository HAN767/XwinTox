#include <FL/Fl.H>
#include <FL/Fl_Scroll.H>

#include "control/cntctlst.h"
#include "control/svgbox.h"

ContactsList::ContactsList(int X, int Y, int W, int H, int S)
	: Fl_Scroll(X, Y, W, H)
{
	scale =S;
	selected =-1;
	color(fl_rgb_color(65, 65, 65));
	type(6);
}

void ContactsList::draw()
{
	Fl_Scroll::draw();

	for(const auto entry : entries)
	{
		entry->icon->redraw();
	}
}

void ContactsList::resize(int X, int Y, int W, int H)
{
	Fl_Scroll::resize(X, Y, W, H);

	for(const auto entry : entries)
	{
		entry->icon->draw();
	}
}

int ContactsList::handle(int event)
{
	switch(event)
	{
	case FL_PUSH:
		if(Fl::event_button() == FL_LEFT_MOUSE)
		{
			for(const auto entry : entries)
			{
				entry->redraw();
				entry->icon->redraw();
			}
		}
	}

	Fl_Scroll::handle(event);
	return 0;
}

void ContactsList::clear_all()
{
	this->clear();
	entries.clear();
	this->redraw();
	parent()->redraw();
}

void ContactsList::deselect_all()
{
	for(const auto entry : entries)
	{
		entry->selected =0;
		entry->icon->show();
		entry->invicon->hide();
		entry->redraw();
		entry->icon->redraw();
	}
}
