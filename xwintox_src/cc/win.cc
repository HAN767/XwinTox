#include <algorithm>
#include <vector>
#include <stdio.h>

#include <FL/Fl.H>
#include <FL/Fl_Box.H>
#include <FL/Fl_Double_Window.H>
#include <FL/Fl_Menu_Bar.H>
#include <FL/Fl_Multiline_Input.H>
#include <FL/Fl_Text_Display.H>
#include <FL/Fl_Scroll.H>
#include <FL/Fl_RGB_Image.H>
#include <FL/fl_draw.H>

#include "contacts.h"
#include "xwintox_win.h"
#include "svgs.h"
#include "nanosvg/nsvgwrap.h"

extern class XwinTox *XwinTox;
extern int CGUIUPDFLAG;

using namespace std;

ContactsEntry::ContactsEntry(int X, int Y, int S, Contact_t *C, Groupchat_t *G, 
							 short T)
 : Fl_Box (X, Y, 224 * S, 50 * S)
{
	scale =S;
	contact =C;
	groupchat =G;
	type =T;
	selected =0;

	if (!T)
	{
		icon =new SVGBox(X+(4 * S), Y+(2 * S), 46 * S, 46 *S, S,
								  default_av, 0.35);
		invicon =new SVGBox(X+(4 * S), Y+(2 * S), 46 * S, 46 *S, S,
								  default_av, 0.35);
		invicon->hide();
		groupchat =new Groupchat_t;
		groupchat->num =65535;
	}
	else
	{
		icon =new SVGBox(X-(8 * S), Y+(2 * S), 46 * S, 46 *S, S,
							  groupsvg, 0.63);
		invicon =new SVGBox(X+(14 * S), Y+(12 * S), 46 * S, 46 *S, S,
							  groupsvg2, 0.63);
		invicon->hide();
		contact =new Contact_t;
		contact->num =65535;
	}

	box(FL_FLAT_BOX);
	color(fl_rgb_color(65, 65, 65));
}

void ContactsEntry::draw()
{
	int txt_color =255;
	char *name, *status;
	if (selected) 
	{ 
		color(255); txt_color =0;
		icon->hide(); invicon->show();
	}
	else
	{
		color (fl_rgb_color(65, 65, 65)); txt_color=255; 
		invicon->hide(); icon->show();
	}

	Fl_Box::draw();
	if (!type)
	{
		name =GetDisplayName(contact, 16); status =GetDisplayStatus(contact, 25);
		if(contact->connected) fl_color(2);
		else fl_color(FL_RED);
		fl_pie(x() + (185 * scale), this->y() + (20 * scale), 10 * scale,
			  10 * scale, 0, 360);
	}
	else
	{
		name =groupchat->name;
		status="";
	}
	fl_color(txt_color);
	fl_font(FL_HELVETICA, 12 * scale);
	fl_draw(name, x() + (50 * scale), y() + (22 * scale));
	fl_font(FL_HELVETICA, 10 * scale);
	fl_draw(status, x() + (50 * scale), y() + (36 * scale));
	icon->draw(); /* make it account for scrollbar::value(); */
	invicon->draw();
}

int ContactsEntry::handle(int event) 
{
	switch(event) 
	{
	case FL_PUSH:
		if ( Fl::event_button() == FL_LEFT_MOUSE )
		{
			for (const auto entry : ((ContactsList*)parent())->entries)
			{
			entry->selected =0;
			entry->redraw(); entry->icon->redraw(); entry->invicon->redraw();
			((Sidebar*)(parent()->parent()))->bottom_area->deselect_all();
			}
			selected =1;
			((ContactsList*)parent())->seltype =type;
			redraw(); icon->redraw(); invicon->redraw();
			if (!type)
			{
				((ContactsList*)parent())->selected =contact->num;
			}
			else
			{ 
				((ContactsList*)parent())->selected =groupchat->num; 
			}
			icon->hide();
			invicon->show();
			if(!type)
			{ XwinTox->contents->NewCurrentArea(FindContactMArea(contact)); }
			else
			{ XwinTox->contents->NewCurrentArea(FindGroupchatMArea(groupchat));}
			return 1;
		}
		else if (Fl::event_button() == FL_RIGHT_MOUSE)
		{
			Fl_Menu_Item contmenu[] = { { "Delete contact" }, { 0 } };
			contmenu->labelsize(12 * scale);
			const Fl_Menu_Item *m = contmenu->popup(Fl::event_x(), Fl::event_y(),
													   0, 0, 0);
			if(!m) return 0;
			else if (strcmp (m->label(), "Delete contact") == 0)
			{
			vector <ContactsEntry*> *ref =&((ContactsList*)parent())->entries;
ref->erase(std::remove(ref->begin(), ref->end(), this), ref->end());
				DeleteContact(contact->num);
				parent()->remove(this);
				Fl::delete_widget(this);
				Fl::delete_widget(FindContactMArea(contact));
				CGUIUPDFLAG =1;
				return 0;
			}
		}
	}
	return 0;
}

ContactsList::ContactsList(int X, int Y, int W, int H, int S) : Fl_Scroll (X, Y, W, H)
{
	scale =S;
	selected =-1;
	color(fl_rgb_color(65, 65, 65));
	type(6);

}

void ContactsList::draw() 
{
	Fl_Scroll::draw();
	for (const auto entry : entries)
	{
		entry->icon->redraw();
	}
}

void ContactsList::resize(int X, int Y, int W, int H) 
{
	Fl_Scroll::resize(X, Y, W, H);
	for (const auto entry : entries)
	{
		entry->icon->draw();
	}
}

int ContactsList::handle(int event) 
{
	switch(event) 
	{
	case FL_PUSH:
	if(Fl::event_button() == FL_LEFT_MOUSE)
	{
		for (const auto entry : entries)
		{
		entry->redraw(); entry->icon->redraw();
		}
	}
	}
	Fl_Scroll::handle(event);
	return 0;
}

void ContactsList::clear_all()
{
	this->clear();
	entries.clear();
	this->redraw();
	parent()->redraw();
}

void ContactsList::deselect_all()
{
	for (const auto entry : entries)
	{
		entry->selected =0;
		entry->icon->show();
		entry->invicon->hide();
		entry->redraw(); entry->icon->redraw();
	}
}

StatusBox::StatusBox(int X, int Y, int W, int H, int S) : Fl_Box (X, Y, W, H)
{
	scale =S;
	box(FL_FLAT_BOX);
	color(fl_rgb_color(65, 65, 65));
}

void StatusBox::draw()
{
	Fl_Box::draw();
	fl_color(2);
	fl_pie(this->x() + (5 * scale), this->y() + (15 * scale), 10 * scale,
			10 * scale, 0, 360);
}


SVGBox::SVGBox(int X, int Y, int W, int H, int S, const char* pic, double factor) : Fl_Box (X, Y, W, H)
{
	img_r =svgrast(pic, W, H, S, factor);
	img =new Fl_RGB_Image(img_r, W, H, 4);
	image(img);
}

void SVGBox::draw()
{
	Fl_Box::draw();
}

int SVGBox::handle(int event) 
{
	switch(event) 
	{
	case FL_PUSH:
		do_callback();
		return 1;
	}
	return 0;
}

void SVGBox::hide()
{
	Fl_Box::hide();
	image(0);
}

void SVGBox::show()
{
	Fl_Box::show();
	image(img);
}

Sidebar_Top_Area::Sidebar_Top_Area(int S) : Fl_Group (XwinTox->basex * S,XwinTox->basey * S,
									   (XwinTox->sblength * S), 
									   60 * S)
{
	scale =S;

	box(FL_FLAT_BOX);
	color(fl_rgb_color(28, 28, 28));

	statusbox =new StatusBox(x() + (192 * S), y() + (10 * S), 20 * S, 40 * S, S);
	avbox =new SVGBox(x() + (10 * S), y() + (10 * S), 40 * S , 40 * S, S, default_av, 0.3);
	name =new Fl_Input(x() + (62 * S), y() + (18 * S), 128 *S, 11 * S);
	status =new Fl_Input(x() + (62 * S), y() + (30 * S), 128 *S, 11 * S);
	name->textsize(12 * S);
	status->textsize(10 * S);
	name->box(FL_FLAT_BOX); name->color(fl_rgb_color(28, 28, 28));
	status->box(FL_FLAT_BOX); status->color(fl_rgb_color(28, 28, 28));
	name->textcolor(255);
	status->textcolor(54);
	name->value("XwinTox User"); status->value("Toxing on XwinTox");

	end();
}

Sidebar_Bottom_Area::Sidebar_Bottom_Area(int S) : Fl_Group (XwinTox->basex * S, XwinTox->h() - (36 * S) - (XwinTox->basey * S),
									   (XwinTox->sblength * S), 
									   XwinTox->h() - (XwinTox->basey * S))
{
	scale =S;

	box(FL_FLAT_BOX);
	color(fl_rgb_color(28, 28, 28));

	addfriend =new SVGBox(0, 0, 56 * scale, 36 * S, S, addfriendsvg, 0.5);
	newgroup =new SVGBox(0, 0, 112 * S , 36 * S, S, groupsvg, 0.5);
	transfers =new SVGBox(0, 0, 168 * S , 36 * S, S, transfersvg, 0.5);
	settings =new SVGBox(0, 0, 212 * S , 36 * S, S, settingssvg, 0.5);

	end();
}

void Sidebar_Bottom_Area::resize(int X, int Y, int W, int H)
{
	Fl_Group::resize(X, Y, W, H);
	addfriend->position(x(), XwinTox->h() - (36 * scale));
	newgroup->position(x() + (56 * scale), XwinTox->h() - (36 * scale));
	transfers->position(x() + (112 * scale), XwinTox->h() - (36 * scale));
	settings->position(x() + (168 * scale), XwinTox->h() - (36 * scale));
}

void Sidebar_Bottom_Area::deselect_all()
{
	addfriend->box(FL_NO_BOX);
	newgroup->box(FL_NO_BOX);
	transfers->box(FL_NO_BOX);
	settings->box(FL_NO_BOX);
}


Sidebar::Sidebar(int S) : Fl_Group (XwinTox->basex * S,XwinTox->basey * S,
									   (XwinTox->sblength * S), 
									   XwinTox->h() - (XwinTox->basey * S))
{
	scale =S;

	box(FL_FLAT_BOX);
	color(fl_rgb_color(65, 65, 65));
	top_area =new Sidebar_Top_Area(S);
	bottom_area =new Sidebar_Bottom_Area(S);
	contacts =new ContactsList(x(), y() + (60 * S), 
							  (XwinTox->sblength * S), h() - (36 * S) - (60 * S), S);

	resize(x(), y(), w(), h());
	end();
}

void Sidebar::resize(int X, int Y, int W, int H)
{
	Fl_Group::resize(X, Y, W, H);
	top_area->resize(XwinTox->basex * scale, XwinTox->basey * scale, 
					 XwinTox->sblength * scale, 60 * scale);
	bottom_area->resize(XwinTox->basex * scale, 
						XwinTox->h() - (36 * scale) - (XwinTox->basey * scale),
						(XwinTox->sblength * scale), 
						XwinTox->h() - (XwinTox->basey * scale));
	contacts->resize(x(), y() + (60 * scale), (XwinTox->sblength * scale),
					 h() - (36 * scale) - (60 * scale));
} 


GArea::GArea(int S, const char *C) : Fl_Group (XwinTox->sblength * S,
									   XwinTox->basey * S,
									   XwinTox->w() - (XwinTox->sblength * S), 
									   XwinTox->h()- (XwinTox->basey * S))
{
	caption =C;
	scale =S;

	box(FL_FLAT_BOX);
	color(255);
}

void GArea::draw()
{
	Fl_Group::draw();

	fl_color(0);
	fl_font(FL_HELVETICA_BOLD, 12 * scale);
	fl_draw(caption, x() + (10 * scale), y() + (24 * scale));

	fl_color(fl_rgb_color(192, 192, 192));
	fl_line(x(), y() + (60 * scale), Fl::w(), y() + (60 * scale));
	fl_color(0);
}

GAddFriend::GAddFriend(int S) : GArea (S, "Add Friends")
{
	scale = S;
	id =new Fl_Input(x() + (10 * S), y() + (90 * S), 
					(x() + w() - (224 * S) - (20 * S)), 
					24 * S);
	message =new Fl_Multiline_Input(x() + (10 * S), y() + (140 * S), 
									(x() + w() - (224 * S) - (20 * S)), 
					84 * S);
	printf("%d\n", Fl::w());

	send =new Fl_Button(parent()->w() - 62 * scale, y() + 234 * scale, 52 * scale,
						20 * scale, "Add");/*Fl::w() - 62 * scale, y() + 234 * scale, 52 * scale,
						20 * scale, "Add");*/

	id->textsize (12 * S); message->textsize (12 * S);
	message->value("Please accept this friend request.");
	send->color(fl_rgb_color(107, 194, 96));
	send->labelcolor(255);
	send->labelsize(14 * S);

	end();
}

void GAddFriend::resize(int X, int Y, int W, int H)
{
	Fl_Group::resize(XwinTox->sblength * scale,
					 XwinTox->basey * scale,
					 XwinTox->w() - (XwinTox->sblength * scale), 
					 XwinTox->h()- (XwinTox->basey * scale));
	id->resize (x() + (10 * scale), y() + (90 * scale), 
				(x() + w() - (224 * scale) - (20 * scale)), 
				24 * scale);
	message->resize (x() + (10 * scale), y() + (140 * scale), 
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

GMessageArea::GMessageArea(int S, Contact_t *C, Groupchat_t *G, short T)
: Fl_Group (0, 0, 0, 0)
{
	contact =C;
	groupchat =G;
	scale =S;
	mtype =T;

	box(FL_FLAT_BOX);
	color(255);

	if (!T)
	{
		icon = new SVGBox(0, 0,  40 * scale,  40 * scale, S, default_av, 0.32);
		groupchat =new Groupchat_t;
		groupchat->num =65535;
	}
	else
	{
		icon = new SVGBox(0, 0,  40 * scale,  40 * scale, S, groupsvg2, 1);
		contact =new Contact_t;
		contact->num =65535;
	}

	names =new Fl_Select_Browser(0, 0, 60 * scale, h() - 80 * scale); 
	gnames =new Fl_Multiline_Output(0,0, w() - (65 * scale), (28 * scale));
	send =new Fl_Button(0, 0, (64 * scale), (64 * scale) , "Send");
	moutput =new Fl_Text_Display(0, 0, 0, 0);
	moutbuffer =new Fl_Text_Buffer();

	names->textsize(9.5 * scale);
	names->textcolor(fl_rgb_color(56, 56, 56));
	names->has_scrollbar(Fl_Browser_::VERTICAL);
	names->box(FL_FLAT_BOX);
	names->hide();

	gnames->box(FL_NO_BOX);
	if (scale <2)gnames->textsize(9 * scale);
	else gnames->textsize(8.8 * scale);
	gnames->wrap(1);
	gnames->textcolor(fl_rgb_color(180, 180, 180));
	gnames->hide();

	message =new Fl_Multiline_Input(0, 0, (w() - (110 * S)), (64 * S));
	message->textsize (12 * S);

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
	Fl_Group::resize(XwinTox->sblength * scale,
					 XwinTox->basey * scale,
					 XwinTox->w() - (XwinTox->sblength * scale), 
					 XwinTox->h()- (XwinTox->basey * scale));


	icon->position(x()+(12 * scale),  y() + (9 * scale));

	if(mtype == 1)
	{
		names->show();

		names->resize(x() + w() - (93 * scale), y() + (63 * scale), 87 * scale, 
					   h() - ((84 + 57)  * scale));
		gnames->resize(x()+(60 * scale), y() + (28 * scale), w() - (65 * scale),
					   (32 * scale));
		moutput->resize(x() + (10 * scale), y() + (65 * scale),
						w() - (110 * scale), h() - ((84 + 65)  * scale));
	}
	else
	{
		moutput->resize(x() + (10 * scale), y() + (65 * scale),
						w() - (20 * scale), h() - ((84 + 65)  * scale));
	}

	send->position(x() + (w() - 74 * scale), y() + (h() - (80 * scale)));
	message->resize (x() + (10 * scale), y() + h() - (80 * scale),
					(w() - (110 * scale)), (64 * scale));


}

void GMessageArea::draw()
{
	if (mtype)
	{
		if (strcmp(gnames->value(), groupchat->peers)) 
		{ gnames->value(groupchat->peers); gnames->redraw(); }
	}

	Fl_Group::draw();

	fl_color(0);
	if (!mtype)
	{
		fl_font(FL_HELVETICA_BOLD, 12 * scale);
		fl_draw(GetDisplayName(contact, 40), x() + (60 * scale), y() + (26 * scale));
		fl_font(FL_HELVETICA_ITALIC, 11 * scale);
		fl_color(fl_rgb_color(56, 56, 56));
		fl_draw(GetDisplayStatus(contact, 60), x() + (60 * scale), y() + (38 * scale));
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
	fl_line(x(), h() + y() - (85 * scale), Fl::w(), h() + y() - (85 * scale));
}

void AddFriendPressed(Fl_Widget* B , void*);

XWContents::XWContents(int S) : Fl_Box(XwinTox->sblength * S,XwinTox->basey * S,
									   XwinTox->w() - (XwinTox->sblength * S), 
									   XwinTox->h() - (XwinTox->basey * S))
{
	scale =S;
	box(FL_FLAT_BOX);
	color(4);

	addfriend =new GAddFriend(S);
	currentarea =addfriend;
	XwinTox->sidebar->bottom_area->addfriend->box(FL_FLAT_BOX);
	XwinTox->sidebar->bottom_area->addfriend->color(fl_rgb_color(68, 68, 67));
}

void XWContents::NewCurrentArea(Fl_Group *G)
{
	currentarea->hide();
	this->newcurrentarea =G;
	currentarea =newcurrentarea;
	currentarea->show();
	currentarea->redraw();
}

void XWContents::draw()
{
}

XwinTox::XwinTox(int w, int h, const char* c, int S) : Fl_Double_Window(w, h, c) 
{
	scale =S;
	basex =0; basey =0; sblength =224;

	if(scale < 2) basey =35;
	else basey =25;

	mbar = new Fl_Menu_Bar(0, 0, w + 1, basey * S);
	mbar->add("File/Quit");
	mbar->add("Edit/Preferences");
	mbar->add("Tools/Switch Profile");
	mbar->add("Help/_&DHT Information"); mbar->add("Help/About XwinTox");

	if (scale < 2) mbar->textsize(12 * S);
	else mbar->textsize(10 * S);

	box(FL_FLAT_BOX);
	color(255);
}

void XwinTox::init2()
{
	sidebar =new Sidebar(scale);
	contents =new XWContents(scale);
	resizable(contents);
}
void XwinTox::resize (int X, int Y, int W, int H)
{
	Fl_Double_Window::resize (X, Y, W, H);
	contents->resize(X, Y, W, H);
}

