#include <stdio.h>

#include <FL/Fl.H>
#include <FL/Fl_Box.H>
#include <FL/Fl_Double_Window.H>
#include <FL/fl_draw.H>


#include "xwintox_win.h"
#include "svgs.h"
#include "nanosvg/nsvgwrap.h"

SVGBox::SVGBox(int X, int Y, int W, int H, int S) : Fl_Box (X, Y, W, H)
{
	img =svgrast(default_av, W, H, S);
}

void SVGBox::draw()
{
	fl_draw_image(img, this->x(), this->y(), this->w(), this->h(), 4, 0);
}

Sidebar_Top_Area::Sidebar_Top_Area(int s) : Fl_Group (0, 0, 224 * s, 60 * s)
{
	scale =s;

	box(FL_FLAT_BOX);
	color(fl_rgb_color(28, 28, 28));

	avbox = new SVGBox (10 * s, 10 * s, 40 * s , 40 * s, s);
}

Sidebar::Sidebar(int s) : Fl_Group (0, 0, 224 * s, 480)
{
	scale =s;

	box(FL_FLAT_BOX);
	color(fl_rgb_color(65, 65, 65));
	top_area = new Sidebar_Top_Area(s);

	show();
}

void Sidebar::resize(int X, int Y, int W, int H)
{
	Fl_Group::resize(X, Y, W, H);
	top_area->resize(0, 0, 224 * scale, 60 * scale);

} 

XWContents::XWContents(int s) : Fl_Group (224 * s, 0, 416, 480)
{
	//fake = new Fl_Box(0,0, 0, 0);
	//resizable(this);
	resizable(0);
	box(FL_FLAT_BOX);
	color(4);
	show();
}

/*void XWContents::resize(int X, int Y, int W, int H)
{
	printf("%d, %d, %d, %d\n");

} */


XwinTox::XwinTox(int w, int h, const char* c, int s) : Fl_Double_Window(w, h, c) 
{
	box(FL_FLAT_BOX);
	color(255);

	sidebar = new Sidebar(s);
	contents = new XWContents(s);

	resizable(contents);
	scale =s;
}

void XwinTox::resize (int X, int Y, int W, int H)
{
	Fl_Double_Window::resize (X, Y, W, H);
	contents->resize(224 * scale, 0, W - (224 * scale), H);
}

