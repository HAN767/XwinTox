#include <FL/Fl_Multiline_Input.H>

#include "cc/xwintox_win.h"
#include "control/gui.h"

GAddFriend::GAddFriend(int S) : GArea(S, "Add Friends")
{
	scale = S;
	id =new Fl_Input(x() + (10 * S), y() + (90 * S),
	                 (x() + w() - (224 * S) - (20 * S)),
	                 24 * S);
	message =new Fl_Multiline_Input(x() + (10 * S), y() + (140 * S),
	                                (x() + w() - (224 * S) - (20 * S)),
	                                84 * S);
	printf("%d\n", Fl::w());

	send =new Fl_Button(parent()->w() - 62 * scale, y() + 234 * scale,
	                    52 * scale, 20 * scale, "Add");

	id->textsize(12 * S);
	message->textsize(12 * S);
	message->value("Please accept this friend request.");
	send->color(fl_rgb_color(107, 194, 96));
	send->labelcolor(255);
	send->labelsize(14 * S);

	end();
}

void GAddFriend::resize(int X, int Y, int W, int H)
{
	Fl_Group::resize(Xw->sblength * scale,
	                 Xw->basey * scale,
	                 Xw->w() - (Xw->sblength * scale),
	                 Xw->h()- (Xw->basey * scale));
	id->resize(x() + (10 * scale), y() + (90 * scale),
	           (x() + w() - (224 * scale) - (20 * scale)),
	           24 * scale);
	message->resize(x() + (10 * scale), y() + (140 * scale),
	                (x() + w() - (224 * scale) - (20 * scale)),
	                84 * scale);
	send->resize(parent()->w() - 62 * scale, y() + 234 * scale, 52 * scale,
	             20 * scale);
}

void GAddFriend::draw()
{
	GArea::draw();

	fl_color(0);
	fl_font(FL_HELVETICA, 12 * scale);
	fl_draw("Tox ID", x() + (10 * scale), y() + 84 * scale);
	fl_draw("Message", x() + (10 * scale), y() + 134 * scale);
}