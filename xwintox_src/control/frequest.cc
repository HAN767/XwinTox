#include <stdio.h>

#include <vector>
using namespace std;

#include <FL/fl_draw.H>
#include <FL/Fl_Box.H>
#include <FL/Fl_Button.H>
#include <FL/Fl_Multiline_Output.H>

extern "C"
{
#include "xwintox.h"
}

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
		me->selected =0;
		me->redraw();
	}
}

FriendRequests::FriendRequests(int X, int Y, int S)
	: Fl_Box(X, Y, (Xw->sblength * S) - 15, frheight * S)
{
	scale =S;
	selected =-1;
	box(FL_BORDER_BOX);
	color(fl_rgb_color(107, 194, 96));

	message =new Fl_Multiline_Output(0, 0, 0, 0);
	message->box(FL_NO_BOX);
	message->textsize(10.8 * scale);
	message->textcolor(255);
	message->value("");
	message->wrap(1);

	PB_Register(postbox, PB_FRequest, this, frs_post);
}

void FriendRequests::resize(int X, int Y, int W, int H)
{
	Fl_Box::resize(X, Y, W, H);
	message->resize(x() + 4 * scale, y() + 20 * scale, w() - 8 * scale, 40 * scale);
}

void FriendRequests::draw()
{
	static char idtext[255] = { 0 };

	if(selected != -1)
	{
		Fl_Box::draw();
		fl_color(255);

		sprintf(idtext, "Client ID: %s",
		        GetShortenedText(frs[selected]->pubkey, 20));
		fl_font(fl_font(), 11.2 * scale);
		fl_draw(idtext, x() + 4 * scale,
		        y() + 14 * scale);
	}
}

void FriendRequests::show()
{
	((Sidebar*)parent())->top2_h +=frheight + 4;
	Fl_Box::show();
}

void FriendRequests::hide()
{
	((Sidebar*)parent())->top2_h -=frheight + 4;
	Fl_Box::hide();
}
