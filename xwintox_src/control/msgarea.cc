#include <FL/Fl_Multiline_Input.H>

#include "xwin/svgs.h"
#include "xwin/contacts.h"

#include "control/gui.h"
#include "control/msgarea.h"
#include "control/svgbox.h"

GMessageArea::GMessageArea(int S, Contact_t *C, Groupchat_t *G, short T)
	: Fl_Group(0, 0, 0, 0)
{
	contact =C;
	groupchat =G;
	scale =S;
	mtype =T;

	box(FL_FLAT_BOX);
	color(255);

	if(!T)
	{
		icon = new SVGBox(0, 0,  40 * scale,  40 * scale, S, default_av, 0.32);
		icon->show();
		groupchat =new Groupchat_t;
		groupchat->num =65535;
	}
	else
	{
		icon = new SVGBox(0, 0,  40 * scale,  40 * scale, S, groupsvg2, 1);
		icon->show();
		contact =new Contact_t;
		contact->num =65535;
	}

	names =new Fl_Select_Browser(0, 0, 60 * scale, h() - 80 * scale);
	gnames =new Fl_Multiline_Output(0,0, w() - (65 * scale), (28 * scale));
	moutput =new Fl_Text_Display(0, 0, 0, 0);
	moutbuffer =new Fl_Text_Buffer();
	send =new Fl_Button(0, 0, (64 * scale), (74 * scale) , "Send");
	message =new Fl_Multiline_Input(0, 0, (w() - (110 * S)), (74 * S));

	names->textsize(9.5 * scale);
	names->textcolor(fl_rgb_color(56, 56, 56));
	names->has_scrollbar(Fl_Browser_::VERTICAL);
	names->box(FL_FLAT_BOX);
	names->hide();

	gnames->box(FL_NO_BOX);

	if(scale <2) gnames->textsize(9 * scale);
	else gnames->textsize(8.8 * scale);

	gnames->wrap(1);
	gnames->textcolor(fl_rgb_color(180, 180, 180));
	gnames->hide();

	message->textsize(12 * S);
	message->wrap(1);

	send->color(fl_rgb_color(107, 194, 96));
	send->labelcolor(255);
	send->labelsize(14 * S);

	moutput->wrap_mode(moutput->WRAP_AT_BOUNDS, 0);
	moutput->box(FL_NO_BOX);
	moutput->textsize(12 * S);
	moutput->buffer(moutbuffer);

	resize(0, 0, 0, 0);
	end();
}

void GMessageArea::resize(int X, int Y, int W, int H)
{
	Fl_Group::resize(Xw->sblength * scale,
	                 Xw->basey * scale,
	                 Xw->w() - (Xw->sblength * scale),
	                 Xw->h()- (Xw->basey * scale));


	icon->position(x() + (12 * scale),  y() + (9 * scale));

	if(mtype == 1)
	{
		names->show();

		names->resize(x() + w() - (93 * scale), y() + (63 * scale),
		              87 * scale, h() - ((84 + 68)  * scale));
		gnames->resize(x() + (60 * scale), y() + (28 * scale), w()
		               - (70 * scale), (32 * scale));
		moutput->resize(x() + (10 * scale), y() + (65 * scale),
		                w() - (110 * scale), h() - ((84 + 65)  * scale));
	}
	else
	{
		moutput->resize(x() + (10 * scale), y() + (65 * scale),
		                w() - (20 * scale), h() - ((84 + 65)  * scale));
	}

	send->position(x() + (w() - 74 * scale), y() + (h() - (80 * scale)));
	message->resize(x() + (10 * scale), y() + h() - (80 * scale),
	                (w() - (110 * scale)), (74 * scale));


}

void GMessageArea::draw()
{
	fl_push_clip(x(), y(), w(), h());
	if(mtype)
	{
		if(strcmp(gnames->value(), groupchat->peers))
		{
			gnames->value(groupchat->peers);
			gnames->redraw();
		}
	}

	Fl_Group::draw();

	fl_color(0);

	if(!mtype)
	{
		fl_font(FL_HELVETICA_BOLD, 12 * scale);
		fl_draw(GetDisplayName(contact, 40), x() + (60 * scale),
		        y() + (26 * scale));
		fl_font(FL_HELVETICA_ITALIC, 11 * scale);
		fl_color(fl_rgb_color(56, 56, 56));
		fl_draw(GetDisplayStatus(contact, 60), x() + (60 * scale),
		        y() + (38 * scale));
	}
	else
	{
		char tmp[128];
		sprintf(tmp, "%d peers in chat", groupchat->num_peers);
		fl_font(FL_HELVETICA_BOLD, 12 * scale);
		fl_draw(groupchat->name, x() + (60 * scale), y() + (26 * scale));
		fl_font(FL_HELVETICA_ITALIC, 11 * scale);
		fl_color(fl_rgb_color(56, 56, 56));
		fl_draw(tmp, x() + (60 * scale), y() + (38 * scale));
		fl_color(fl_rgb_color(192, 192, 192));
		fl_line(x() + w() - (95 * scale), y() + (60 * scale),
		        x() + w() - (95 * scale),y() + h() - ((85)  * scale));
	}


	fl_color(fl_rgb_color(192, 192, 192));
	fl_line(x(), y() + (60 * scale), Fl::w(), y() + (60 * scale));
	fl_line(x(), h() + y() - (85 * scale), Fl::w(), h() + y() -
	        (85 * scale));

	fl_pop_clip();
}
