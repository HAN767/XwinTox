#ifndef XWINTOX_WIN_H_
#define XWINTOX_WIN_H_

#include <FL/Fl.H>
#include <FL/Fl_Box.H>
#include <FL/Fl_Double_Window.H>
#include <FL/Fl_Input.H>
#include <FL/Fl_RGB_Image.H>

class StatusBox: public Fl_Box
{
public:
	// Ctor
	StatusBox(int X, int Y, int W, int H, int S);
	void draw();

	int scale;
	int status; /* 1 is green, 2 is red, 3 is yellow, 4 is grey */
};

class SVGBox: public Fl_Box
{
public:
	// Ctor
	SVGBox(int X, int Y, int W, int H, int S);

	int scale;
	unsigned char *img_r;
	Fl_RGB_Image *img;
};

class Sidebar_Top_Area: public Fl_Group
{
public:
	// Ctor
	Sidebar_Top_Area(int S);

	SVGBox *avbox;
	Fl_Input *name;
	Fl_Input *status;
	StatusBox *statusbox;


	int scale;
};


class Sidebar: public Fl_Group
{
public:
	// Ctor
	Sidebar(int S);

	void resize (int X, int Y, int W, int H);

	Sidebar_Top_Area *top_area;

	int scale;
};

class XWContents: public Fl_Group
{
public:
	// Ctor
	XWContents(int S);
	Fl_Box *fake;
};

class XwinTox : public Fl_Double_Window
{
public:
	// Ctor
	XwinTox(int w, int h, const char* c, int S);

	void resize (int X, int Y, int W, int H);

	Sidebar *sidebar;
	XWContents *contents;

	int scale;
};

#endif
