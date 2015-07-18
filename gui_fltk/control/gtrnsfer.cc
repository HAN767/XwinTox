#include <ctype.h>

#include <FL/Fl.H>
#include <FL/Fl_Multiline_Input.H>
#include <FL/fl_draw.H>

#include "misc.h"

#include "control/xwin.h"
#include "control/gtrnsfer.h"
#include "control/translst.h"
#include "control/transent.h"

GTransfers::GTransfers(const XWF_hObj_t* hObj, int S) :
	GArea(hObj, S, "File Transfers")
{
	scale = S;

	list =new TransfersList(0, 0, 1, 1, scale);

	resize(x(), y(), w(), h());
	end();
}

void GTransfers::resize(int X, int Y, int W, int H)
{
	Fl_Group::resize(X, Y, W, H);
	list->resize(x(), y() + (61 * scale), w(), h() - (61 * scale));
}

void GTransfers::draw()
{
	GArea::draw();
}
