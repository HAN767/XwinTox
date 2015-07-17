#include <ctype.h>

#include <FL/Fl.H>
#include <FL/Fl_Multiline_Input.H>
#include <FL/fl_draw.H>

#include "misc.h"

#include "control/xwin.h"
#include "control/gtrnsfer.h"
#include "control/translst.h"
#include "control/transent.h"

void gt_post(int mtype, PBMessage_t* msg, void* custom)
{
	/*GTransfers *g =(GTransfers*)custom;

	if(mtype == PB_TRequest)
	{
		Transfer_t *newtransfer =new Transfer_t;
		time_t rawtime;

		time(&rawtime);
		newtransfer->time =localtime(&rawtime);
		newtransfer->state =TR_Waiting;
		newtransfer->dir =TR_Recv;
		newtransfer->contact =FindContact(msg->I1);
		newtransfer->num =msg->I2;
		newtransfer->size =msg->I3;
		newtransfer->filename =strdup(msg->S1);
		g->transfers.push_back(newtransfer);
		dbg("Transfer: %s from %s\n", newtransfer->filename, newtransfer->contact->name);
	}

	g->regengui =1;
	g->redraw();*/
}

GTransfers::GTransfers(const XWF_hObj_t* hObj, int S) :
	GArea(hObj, S, "File Transfers")
{
	scale = S;

	list =new TransfersList(0, 0, 1, 1, scale);

	//PB_Register(APP->events, PB_TRequest, this, gt_post);

	resize(x(), y(), w(), h());
	end();
}

void GTransfers::resize(int X, int Y, int W, int H)
{
/*	Fl_Group::resize(Xw->sblength * scale,
	                 Xw->basey * scale,
	                 Xw->w() - (Xw->sblength * scale),
	                 Xw->h()- (Xw->basey * scale));*/
	Fl_Group::resize(X, Y, W, H);
	list->resize(x(), y() + (61 * scale), w(), h() - (61 * scale));
}

void GTransfers::draw()
{
	if(regengui)
	{
		regengui =0;
		regen_gui();
	}

	GArea::draw();
}

void GTransfers::regen_gui()
{
	int YM =list->y(), XM =list->x(), inv =0;

	/* delete the old ones */
	list->clear_all();

/*	for(const auto t : transfers)
	{
		TransfersEntry *te =new TransfersEntry(XM, YM, scale, t, inv);
		dbg("Regen for Trans %d %d\n", t->contact->wNum, t->num);

		list->entries.push_back(te);
		list->add(te);
		te->show();
		YM += 50;
		inv =1 - inv;
	}*/

	list->redraw();
	redraw();

	return;
}
