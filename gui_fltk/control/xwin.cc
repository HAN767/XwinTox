#include <FL/Fl_Double_Window.H>
#include <FL/Fl_Group.H>

#include "control/xwin.h"
#include "control/sidebar.h"
#include "misc.h"
#include "GUIFLTK.h"
//#include "control/svgbox.h"

XWContents::XWContents(const XWF_hObj_t* hObj, int S) : Fl_Box(0, 0, 1, 1)
{
	scale =S;
	box(FL_FLAT_BOX);
	color(4);
	hObj_ =hObj;

	addfriend =new GAddFriend(hObj_, S);
	transfers =new GTransfers(hObj_, S);
	settings =new GSettings(hObj_, S);

	transfers->hide();
	settings->hide();

	currentarea =addfriend;
}

void XWContents::NewCurrentArea(Fl_Group *G)
{
	Fl_Widget *w =this->parent();
	currentarea->hide();
	this->newcurrentarea =G;
	currentarea =newcurrentarea;
	currentarea->show();
	currentarea->redraw();

	w->resize(w->x(), w->y(), w->w(), w->h());
}

void XWContents::draw()
{
}

void handle(void *userdata)
{
	userdata_t *ud =(userdata_t*)userdata;
	if(ud->op == 'U')
	{
		ContactsList *ctLst =(ContactsList*)ud->lptr;
		updatecontacts(ctLst, ctLst->lstContacts);
	}
}

XwinTox::XwinTox(GUIFLTK *G, int w, int h, const char* c, int S)
	: Fl_Double_Window(w, h, c)
{
	scale =S;
	Obj =G;
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
	sidebar =new Sidebar(Obj->hObj_, scale);
	contents =new XWContents(Obj->hObj_, scale);
	sidebar->bottom_area->addfriend->box(FL_FLAT_BOX);
	sidebar->bottom_area->addfriend->color(fl_rgb_color(68, 68, 67));
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
	contents->transfers->resize(sblength * scale, basey * scale,
	                            W - (sblength * scale), H- (basey * scale));
	contents->settings->resize(sblength * scale, basey * scale,
	                            W - (sblength * scale), H- (basey * scale));
	for (const auto entry : contents->messageareas)
	{
		entry->resize(sblength * scale, basey * scale,
	                 w() - (sblength * scale), h() - (basey * scale));
	}
}
