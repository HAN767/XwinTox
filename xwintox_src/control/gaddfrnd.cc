#include <ctype.h>

#include <FL/Fl.H>
#include <FL/Fl_Multiline_Input.H>
#include <FL/fl_draw.H>

#include "control/gui.h"
#include "control/gaddfrnd.h"

void removespaces(char * s)
{
    char *p =s;
    int l =strlen(p);

    while(isspace(p[l - 1])) p[--l] =0;
    while(*p && isspace(*p)) ++p, --l;

    memmove(s, p, l + 1);
}

void af_pressed(Fl_Widget *w, void *custom)
{
	GAddFriend *g =(GAddFriend*)custom;
	char *amsg =(char*)calloc(255, sizeof(char));
	char *bmsg =(char*)calloc(9, sizeof(char));
	char *cmsg =(char*)calloc(14, sizeof(char));
	char *id =strdup(g->id->value());
	const char *msg =g->message->value();

	removespaces(id);

	/* try to identify what type of request this is */
	if ((strlen(id) == 76) && !strchr(id, '@'))
	{
		/* a regular tox key */
		dbg("Regular Tox key\n");
	}

	sprintf(amsg, "sendfriendrequest %s %s", id, msg);
	free (id);
	strcpy(bmsg, "savedata");
	strcpy(cmsg, "getfriendlist");

	List_add(APP->Comm->WorkQueue, (void*)amsg);
	List_add(APP->Comm->WorkQueue, (void*)bmsg);
	List_add(APP->Comm->WorkQueue, (void*)cmsg);
	CommWork();
}

GAddFriend::GAddFriend(int S) : GArea(S, "Add Friends")
{
	scale = S;
	id =new Fl_Input(x() + (10 * S), y() + (90 * S),
	                 (x() + w() - (224 * S) - (20 * S)),
	                 24 * S);
	message =new Fl_Multiline_Input(x() + (10 * S), y() + (140 * S),
	                                (x() + w() - (224 * S) - (20 * S)),
	                                84 * S);

	send =new Fl_Button(parent()->w() - 62 * scale, y() + 234 * scale,
	                    52 * scale, 20 * scale, "Add");


	id->textsize(12 * S);

	message->textsize(12 * S);
	message->value("Please accept this friend request.");

	send->color(fl_rgb_color(107, 194, 96));
	send->labelcolor(255);
	send->labelsize(14 * S);
	send->callback(af_pressed, this);

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
