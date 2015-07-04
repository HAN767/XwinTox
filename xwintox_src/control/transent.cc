#include <math.h>

#include <FL/fl_draw.H>
#include <FL/Fl_Group.H>

#include "xwintox.h"

#include "control/gui.h"
#include "control/gtrnsfer.h"
#include "control/transent.h"

const char *GetDisplaySize(unsigned int bytes)
{
	static char dsize[255] ={ 0 };
	const char *suffixes[5] ={ "B", "KB", "MB", "GB", "TB" };
	double s =bytes;
	int o =0, p = 2;

	while (s >= 1024 && o + 1 < bytes)
	{
		o ++;
		s =s / 1024;
	}

    if (s - floor(s) == 0.0) p = 0;
	else if (s - floor(s) <= 0.1) p = 1;
	sprintf(dsize, "%.*f %s", p, s, suffixes[o]);
	return dsize;
}

TransfersEntry::TransfersEntry(int X, int Y, int S, Transfer_t *T, int I)
	: Fl_Group(X, Y, Xw->w() - (Xw->sblength * S), 50 * S)
{
	scale =S;
	transfer =T;
	inv =I;

	accept =new Fl_Button(0, 0, 1, 1, "Accept");
	reject =new Fl_Button(0, 0, 1, 1, "Reject");

	accept->color(fl_rgb_color(118, 202, 116));
	reject->color(fl_rgb_color(214, 78, 77));

	accept->labelsize(11 * scale);
	reject->labelsize(11 * scale);

	box(FL_FLAT_BOX);

	if(I == 1) color(fl_rgb_color(198, 199, 214));
	else color(fl_rgb_color(239, 239, 239));

	resize(X, Y, w(), h());
	end();
}

void TransfersEntry::resize(int X, int Y, int W, int H)
{
	Fl_Group::resize(X, Y, W, H);
	accept->resize(X + w() - (78 * scale), y() + (2 * scale), 54 * scale,
	               (h() / 2) - 2 * scale);
	reject->resize(X + w() - (78 * scale), y() + (h() / 2) + (2 * scale),
	               54 * scale,(h() / 2) - 4 * scale);
}

void TransfersEntry::draw()
{
	char from[255] = { 0 };

	sprintf(from, "From: %s", GetDisplayName(transfer->contact, 100));

	Fl_Group::draw();

	fl_color(fl_rgb_color(110, 100, 118));
	fl_font(FL_HELVETICA_BOLD, 12 * scale);
	fl_draw(transfer->filename, x() + (10 * scale), y() + (16 * scale));

	fl_font(FL_HELVETICA, 11 * scale);
	fl_draw(from, x() + (10 * scale), y() + (30 * scale));

	fl_draw(GetDisplaySize(transfer->size), x() + (10 * scale), y() + (44 * scale));
}
