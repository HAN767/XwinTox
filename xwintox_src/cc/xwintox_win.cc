#include <stdio.h>

#include <FL/Fl.H>
#include <FL/Fl_Box.H>
#include <FL/Fl_Double_Window.H>
#include <FL/Fl_Multiline_Input.H>
#include <FL/Fl_RGB_Image.H>
#include <FL/fl_draw.H>

#include "xwintox_win.h"
#include "svgs.h"
#include "nanosvg/nsvgwrap.h"

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

Sidebar_Top_Area::Sidebar_Top_Area(int S) : Fl_Group (0, 0, 224 * S, 60 * S)
{
	scale =S;

	box(FL_FLAT_BOX);
	color(fl_rgb_color(28, 28, 28));

	statusbox =new StatusBox(192 * S, 10 * S, 20 * S, 40 * S, S);
	avbox =new SVGBox(10 * S, 10 * S, 40 * S , 40 * S, S, default_av, 0.3);
	name =new Fl_Input(62 * S, 18 * S, 128 *S, 10 * S);
	status =new Fl_Input(62 * S, 30 * S, 128 *S, 10 * S);
	name->textsize(12 * S);
	status->textsize(10 * S);
	name->box(FL_FLAT_BOX); name->color(fl_rgb_color(28, 28, 28));
	status->box(FL_FLAT_BOX); status->color(fl_rgb_color(28, 28, 28));
	name->textcolor(255);
	status->textcolor(54);
	name->value("XwinTox User"); status->value("Toxing on XwinTox");

	end();
}

Sidebar_Bottom_Area::Sidebar_Bottom_Area(int S) : Fl_Group (0, 
										(480 * S) - (36 * S), 224 * S, 480 * S)
{
	scale =S;

	box(FL_FLAT_BOX);
	color(fl_rgb_color(28, 28, 28));

	addfriend =new SVGBox(0, (480 * S) - (36 * S), 56 * S , 36 * S, S, addfriendsvg, 0.5);
	newgroup =new SVGBox(56 * S, (480 * S) - (36 * S), 112 * S , 36 * S, S, groupsvg, 0.5);
	transfers =new SVGBox(112 * S, (480 * S) - (36 * S), 168 * S , 36 * S, S, transfersvg, 0.5);
	settings =new SVGBox(168 * S, (480 * S) - (36 * S), 212 * S , 36 * S, S, settingssvg, 0.5);

	end();
}


Sidebar::Sidebar(int S) : Fl_Group (0, 0, 224 * S, 480 * S)
{
	scale =S;

	box(FL_FLAT_BOX);
	color(fl_rgb_color(65, 65, 65));
	top_area =new Sidebar_Top_Area(S);
	bottom_area =new Sidebar_Bottom_Area(S);
	end();
}

void Sidebar::resize(int X, int Y, int W, int H)
{
	Fl_Group::resize(X, Y, W, H);
	top_area->resize(0, 0, 224 * scale, 60 * scale);
	bottom_area->resize(0, H - (36 * scale), 224 * scale, 480 * scale);
} 


GArea::GArea(int S, const char *C) : Fl_Group (224 * S, 0, 416 * S, 480 * S)
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
	Fl_Group::resize(X, Y, W, H);
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

XWContents::XWContents(int S) : Fl_Box (224 * S, 0, 416 * S, 480 * S)
{
	scale =S;
	//resizable(0);
	box(FL_FLAT_BOX);
	color(4);

	addfriend =new GAddFriend(S);
	
}


XwinTox::XwinTox(int w, int h, const char* c, int S) : Fl_Double_Window(w, h, c) 
{
	scale =S;

	box(FL_FLAT_BOX);
	color(255);


	contents =new XWContents(S);
	sidebar =new Sidebar(S);
	resizable(contents);
}

void XwinTox::resize (int X, int Y, int W, int H)
{
	Fl_Double_Window::resize (X, Y, W, H);
	contents->resize(224 * scale, 0, W - (224 * scale), H);
}

