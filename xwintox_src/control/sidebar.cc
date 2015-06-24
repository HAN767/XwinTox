#include <stdio.h>

#include "control/gui.h"
#include "control/sidebar.h"

#include "c/signal.h"

void sb_post(int mtype, PBMessage_t* msg, void* custom)
{
	if(mtype == PB_FRequest)
	{
		static char newlabel[32];
		Sidebar *me =(Sidebar*) custom;
		me->frs +=1;
		if(me->frs > 1) sprintf(newlabel, "%d Friend\n Requests", me->frs);
		else sprintf(newlabel, "1 Friend\nRequest");

		me->frbutton->label(newlabel);
	}
	dbg("Received post: %s, %s\n", msg->S1, msg->S2);
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
	f_reqs->show();
	//f_reqs->hide();
	frbutton->labelsize(10 * scale);
	frbutton->label("No Friend\nRequests");
	frbutton->color(fl_rgb_color(107, 194, 96));
	frbutton->labelcolor(255);
	
	PB_Register(postbox, PB_FRequest, this, sb_post);

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

	f_reqs->resize(x() + 3, y() + 94 + 2, (Xw->sblength * scale) - (15 * scale),
	               frheight * scale);

	contacts->resize(x(), y() + ((60 + top2_h) * scale), (Xw->sblength * scale),
	                 h() - (36 * scale) - (60 * scale) - (top2_h * scale));
}
