#ifndef SVGBOX_H_
#define SVGBOX_H_

#include <FL/Fl_Box.H>
#include <FL/Fl_RGB_Image.H>

class SVGBox: public Fl_Box
{
public:
	// Ctor
	SVGBox(int X, int Y, int W, int H, int S, const char* pic, double factor);
	void draw();
	int handle(int event);
	void hide();
	void show();

	int scale;
	unsigned char *img_r;
	Fl_RGB_Image *img;
	Fl_Color bg;
};

#endif
