#include <FL/Fl_Double_Window.H>
#include <FL/Fl_Group.H>

#include "control/xwin.h"
#include "control/sidebar.h"
//#include "control/svgbox.h"

XWContents::XWContents(int X, int Y, int W, int H, int S) : Fl_Box(X, Y, W, H)
{
	scale =S;
	box(FL_FLAT_BOX);
	color(4);

	addfriend =new GAddFriend(S);
	/*transfers =new GTransfers(S);
	transfers->hide();
	currentarea =addfriend;
	Xw->sidebar->bottom_area->addfriend->box(FL_FLAT_BOX);
	Xw->sidebar->bottom_area->addfriend->color(fl_rgb_color(68, 68, 67));*/
}

void XWContents::NewCurrentArea(Fl_Group *G)
{
	currentarea->hide();
	this->newcurrentarea =G;
	currentarea =newcurrentarea;
	currentarea->show();
	currentarea->redraw();
}

void XWContents::draw()
{
}

XwinTox::XwinTox(int w, int h, const char* c, int S)
	: Fl_Double_Window(w, h, c)
{
	scale =S;
	basex =0;
	basey =0;
	sblength =224;

	if(scale < 2) basey =35;
	else basey =25;

	mbar = new Fl_Menu_Bar(0, 0, w + 1, basey * S);
	mbar->add("File/Quit");
	mbar->add("Edit/Preferences");
	mbar->add("Tools/Switch Profile");
	mbar->add("Help/_&DHT Information");
	mbar->add("Help/About XwinTox");

	if(scale < 2) mbar->textsize(12 * S);
	else mbar->textsize(10 * S);

	box(FL_FLAT_BOX);
	color(255);
	init2();
}

void XwinTox::init2()
{
	sidebar =new Sidebar(0, 0, 1, 1, scale);
	contents =new XWContents(0, 0, 1, 1, scale);
	resizable(contents);
	resize(x(), y(), w(), h());
}

void XwinTox::resize(int X, int Y, int W, int H)
{
	Fl_Double_Window::resize(X, Y, W, H);
	contents->resize(sblength * scale, basey * scale,
	                 w() - (sblength * scale), h() - (basey * scale));
	sidebar->resize(basex * scale, basey * scale, sblength * scale,
	                H - (basey * scale));
	contents->addfriend->resize(sblength * scale, basey * scale,
	                  W - (sblength * scale), H- (basey * scale));
}
