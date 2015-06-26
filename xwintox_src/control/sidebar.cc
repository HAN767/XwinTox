#include <stdio.h>

#include "postbox.h"
#include "xwin/contacts.h"

#include "control/gui.h"
#include "control/sidebar.h"

void sb_post(int mtype, PBMessage_t* msg, void* custom)
{
	static char newlabel[32];
	Sidebar *me =(Sidebar*) custom;

	if(mtype == PB_FRequest) me->frs +=1;
	else if(mtype == PB_FReqServiced) me->frs -=1;

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
	CGUIUPDFLAG =1;
	CommWork();
}

Sidebar::Sidebar(int S) : Fl_Group(Xw->basex * S,Xw->basey * S,
	                                   (Xw->sblength * S),
	                                   Xw->h() - (Xw->basey * S))
{
	scale =S;
	top2_h =38;
	frs =0;

	box(FL_FLAT_BOX);
	color(fl_rgb_color(65, 65, 65));
	top_area =new Sidebar_Top_Area(S);
	bottom_area =new Sidebar_Bottom_Area(S);
	contacts =new ContactsList(x(), y() + ((60 + 36) * S),(Xw->sblength * S),
	                           h() - (36 * S) - (60 * S), S);
	contacts->end();

	frbutton =new Fl_Button(0, 0, 0, 0);
	f_reqs =new FriendRequests(x() + 3, y() + 94 + 2, scale);

	frbutton->labelsize(10 * scale);
	frbutton->label("0 Friend\nRequests");
	frbutton->color(fl_rgb_color(107, 194, 96));
	frbutton->labelcolor(255);
	frbutton->callback(frcallback);

	f_reqs->show();
	f_reqs->hide();

	PB_Register(postbox, PB_FRequest | PB_FReqServiced, this, sb_post);

	resize(x(), y(), w(), h());
	end();
}

void Sidebar::resize(int X, int Y, int W, int H)
{
	Fl_Group::resize(Xw->basex * scale,Xw->basey * scale,
	                 (Xw->sblength * scale),
	                 Xw->h() - (Xw->basey * scale));
	top_area->resize(Xw->basex * scale, Xw->basey * scale,
	                 Xw->sblength * scale, 60 * scale);
	bottom_area->resize(Xw->basex * scale,
	                    Xw->h() - (36 * scale) - (Xw->basey * scale),
	                    (Xw->sblength * scale),
	                    Xw->h() - (Xw->basey * scale));

	frbutton->resize(x() + w() - (72  * scale), y() + (64 * scale),
	                 60 * scale, 30 * scale);

	f_reqs->resize(x() + 3 * scale, y() + (94 + 2) * scale,
	               (Xw->sblength * scale) - (15 * scale), frheight * scale);

	contacts->resize(x(), y() + ((60 + top2_h) * scale), (Xw->sblength * scale),
	                 h() - (36 * scale) - (60 * scale) - (top2_h * scale));
}
