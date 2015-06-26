#include <stdio.h>

#include <vector>
#include <algorithm>
using namespace std;

#include <FL/fl_draw.H>
#include <FL/Fl_Box.H>
#include <FL/Fl_Button.H>
#include <FL/Fl_Multiline_Output.H>

#include "xwintox.h"

#include "control/gui.h"
#include "control/frequest.h"

char *GetShortenedText(char* text, size_t LenLimit);

void frs_post(int mtype, PBMessage_t* msg, void* custom)
{
	FriendRequests* me =(FriendRequests*)custom;

	if(mtype == PB_FRequest)
	{
		FriendRequest_t *newfr =new FriendRequest_t;
		newfr->pubkey =strdup(msg->S1);
		newfr->message =strdup(msg->S2);
		me->frs.push_back(newfr);

		if(me->selected == -1) me->selected =0;
		else me->next->activate();

		me->redraw();
	}
}

void frs_nextreq(FriendRequests *f)
{
	unsigned long cnt =f->frs.size();

	if(f->frs.empty())
	{
		dbg("Select -1\n");
		f->selected =-1;
		f->message->value("");
		goto redraw;
	}

	f->selected += 1;

	if(f->selected >= cnt) f->selected =0;

redraw:
	dbg("F->Selected = %d\n", f->selected);

	f->redraw();
}

void frnextcallback(Fl_Widget *w, void *fr)
{
	FriendRequests *f =(FriendRequests*)fr;

	frs_nextreq(f);
}

void fracceptcallback(Fl_Widget *w, void *fr)
{
	char *icmsg =(char*)calloc(255, sizeof(char));
	FriendRequests *f =(FriendRequests*)fr;
	PBMessage_t *message =(PBMessage_t*)calloc(1, sizeof(PBMessage_t));

	sprintf(icmsg, "addfriendnorequest %s", f->frs[f->selected]->pubkey);
	List_add(APP->Comm->WorkQueue, (void*)icmsg);
	CommWork();

	f->frs.erase(f->frs.begin() + f->selected);
	frs_nextreq(f);
	PB_Signal(postbox, PB_FReqServiced, message);
}

FriendRequests::FriendRequests(int X, int Y, int S)
	: Fl_Box(X, Y, (Xw->sblength * S) - 15, frheight * S)
{
	scale =S;
	selected =-1;
	box(FL_BORDER_BOX);
	color(fl_rgb_color(107, 194, 96));

	message =new Fl_Multiline_Output(0, 0, 0, 0);
	accept =new Fl_Button(0, 0, 0, 0, "Accept");
	reject =new Fl_Button(0, 0, 0, 0, "Reject");
	next =new Fl_Button(0, 0, 0, 0, "Next");

	accept->labelsize(11.2 * scale);
	accept->labelcolor(255);
	accept->color(fl_rgb_color(107, 194, 96));
	accept->callback(fracceptcallback, this);

	reject->labelsize(11.2 * scale);
	reject->labelcolor(255);
	reject->color(fl_rgb_color(200, 77, 79));

	next->labelsize(11.2 * scale);
	next->labelcolor(255);
	next->color(fl_rgb_color(79, 77, 200));
	next->callback(frnextcallback, this);

	message->box(FL_NO_BOX);
	message->textsize(10.8 * scale);
	message->textcolor(255);
	message->value("");
	message->wrap(1);

	/*static FriendRequest_t test, test2;
	test.message="<SylvieLorxu> I would definitely not accept stqism as my boyfriend. Sorry, he's just not my type. ";
	test.pubkey="Zetok";
	test2.message="Not a friendo";
	test2.pubkey="Tox";

	frs.push_back(&test);
	frs.push_back(&test2);
	this->selected =0;
	this->redraw();*/

	PB_Register(postbox, PB_FRequest | PB_FReqServiced, this, frs_post);
}

void FriendRequests::resize(int X, int Y, int W, int H)
{
	Fl_Box::resize(X, Y, W, H);
	message->resize(x() + 4 * scale, y() + 20 * scale,
	                w() - 8 * scale, 40 * scale);
	accept->resize(x() + 12 * scale, y() + 65 * scale,
	               w() / 4.5, 18 * scale);
	reject->resize(x() + (w() / 3) + 12 * scale, y() + 65 * scale,
	               w() / 4.5, 18 * scale);
	next->resize(x() + (w() / 3) * 2 + 12 * scale, y() + 65 * scale,
	             w() / 4.5, 18 * scale);
}

void FriendRequests::draw()
{
	static char idtext[255] = { 0 };

	Fl_Box::draw();
	fl_color(255);


	if(selected != -1)
	{
		accept->activate();
		reject->activate();

		if(frs.size() > 1) next->activate();
		else next->deactivate();

		sprintf(idtext, "Client ID: %s",
		        GetShortenedText(frs[selected]->pubkey, 20));
		fl_font(fl_font(), 11.2 * scale);
		fl_draw(idtext, x() + 4 * scale,
		        y() + 14 * scale);
		message->value(frs[selected]->message);
		message->redraw();
	}
	else
	{
		accept->deactivate();
		reject->deactivate();
		next->deactivate();
		fl_draw("No friend requests pending", x() + 4 * scale,
		        y() + 14 * scale);
	}

	accept->redraw();
	reject->redraw();
}

void FriendRequests::show()
{
	((Sidebar*)parent())->top2_h +=frheight + 4;
	message->show();
	accept->show();
	reject->show();
	next->show();
	Fl_Box::show();
}

void FriendRequests::hide()
{
	((Sidebar*)parent())->top2_h -=frheight + 4;
	message->hide();
	accept->hide();
	reject->hide();
	next->hide();
	Fl_Box::hide();
}
