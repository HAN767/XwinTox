#ifndef XWINTOX_WIN_H_
#define XWINTOX_WIN_H_

#include <FL/Fl.H>
#include <FL/Fl_Box.H>
#include <FL/Fl_Double_Window.H>

class SVGBox: public Fl_Box
{
public:
	// Ctor
	SVGBox(int X, int Y, int W, int H, int S);
	void draw();

	int scale;
	unsigned char *img;
};

class Sidebar_Top_Area: public Fl_Group
{
public:
	// Ctor
	Sidebar_Top_Area(int s);

	SVGBox *avbox;

	int scale;
};


class Sidebar: public Fl_Group
{
public:
	// Ctor
	Sidebar(int s);

	void resize (int X, int Y, int W, int H);

	Sidebar_Top_Area *top_area;

	int scale;
};

class XWContents: public Fl_Group
{
public:
	// Ctor
	XWContents(int s);
	Fl_Box *fake;
};

class XwinTox : public Fl_Double_Window
{
public:
	// Ctor
	XwinTox(int w, int h, const char* c, int s);

	void resize (int X, int Y, int W, int H);

	Sidebar *sidebar;
	XWContents *contents;

	int scale;
};

#endif
