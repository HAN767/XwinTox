#include <FL/Fl_Box.H>

#include "nanosvg/nsvgwrap.h"

#include "control/svgbox.h"


SVGBox::SVGBox(int X, int Y, int W, int H, int S, const char* pic,
               double factor) : Fl_Box(X, Y, W, H)
{
	img_r =svgrast(pic, W, H, S, factor);
	img =new Fl_RGB_Image(img_r, W, H, 4);
}

void SVGBox::draw()
{
	Fl_Box::draw();
}

int SVGBox::handle(int event)
{
	switch(event)
	{
	case FL_PUSH:
		do_callback();
		return 1;
	}

	return 0;
}

void SVGBox::hide()
{
	Fl_Box::hide();
	image(0);
}

void SVGBox::show()
{
	Fl_Box::show();
	image(img);
}
