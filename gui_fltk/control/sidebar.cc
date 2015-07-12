#include <stdio.h>

#include "postbox.h"
#include "misc.h"

#include "control/xwin.h"
#include "control/sidebar.h"

void sb_post(int mtype, PBMessage_t* msg, void* custom)
{
	static char newlabel[32];
	Sidebar *me =(Sidebar*) custom;

	//if(mtype == PB_FRequest) me->frs +=1;
	//else if(mtype == PB_FReqServiced) me->frs -=1;

	if(me->frs > 1) sprintf(newlabel, "%d Friend\n Requests", me->frs);
	else if (me->frs == 1) sprintf(newlabel, "1 Friend\nRequest");
	else sprintf(newlabel, "0 Friend\n Requests");

	me->frbutton->label(newlabel);
	me->frbutton->redraw();
	dbg("Received post: %s, %s\n", msg->S1, msg->S2);
}

void frcallback(Fl_Widget *w)
{
	Sidebar *s =((Sidebar*)w->parent());
	FriendRequests *f =s->f_reqs;
	dbg("FR Callback\n");

	if(f->visible()) f->hide();
	else f->show();

	s->resize(s->x(), s->y(), s->w(), s->h());
	//CGUIUPDFLAG =1;
	//CommWork();
}

Sidebar::Sidebar(const XWF_hObj_t* hObj, int S) : Fl_Group(0, 0, 0, 0)
{
	scale =S;
	top2_h =38;
	frs =0;
	hObj_ =hObj;

	box(FL_FLAT_BOX);
	color(fl_rgb_color(65, 65, 65));
	top_area =new Sidebar_Top_Area(hObj_, S);
	bottom_area =new Sidebar_Bottom_Area(hObj_, S);
	/*contacts =new ContactsList(0, 0, 1, 1, S);
	contacts->end();

	frbutton =new Fl_Button(0, 0, 0, 0);
	f_reqs =new FriendRequests(x() + 3, y() + 94 + 2, scale);

	frbutton->labelsize(10 * scale);
	frbutton->label("0 Friend\nRequests");
	frbutton->color(fl_rgb_color(107, 194, 96));
	frbutton->labelcolor(255);
	frbutton->callback(frcallback);*/

	//f_reqs->show();
	//f_reqs->hide();

	//PB_Register(APP->events, PB_FRequest | PB_FReqServiced, this, sb_post);

	resize(x(), y(), w(), h());
	end();
}

void Sidebar::resize(int X, int Y, int W, int H)
{
	Fl_Group::resize(X, Y, W, H);
	top_area->resize(X, Y, W, 60 * scale);
	bottom_area->resize(X, Y + H - (36 * scale) , W, 36 * scale);/*

	frbutton->resize(X + W - (72  * scale), y() + (64 * scale),
	                 60 * scale, 30 * scale);

	f_reqs->resize(X + 3 * scale, Y + (96) * scale, W - (15 * scale), frheight * scale);

	contacts->resize(x(), y() + ((60 + top2_h) * scale), W,
	                 h() - (36 * scale) - (60 * scale) - (top2_h * scale));*/
}
