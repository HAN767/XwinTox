#include <math.h>

#include <FL/fl_draw.H>
#include <FL/Fl_Group.H>
#include "xwintox.h"

#include "control/xwin.h"
#include "control/gtrnsfer.h"
#include "control/transent.h"
#include "util.h"

const char *GetDisplaySize(unsigned int bytes)
{
	static char dsize[255] = { 0 };
	const char *suffixes[5] = { "B", "KB", "MB", "GB", "TB" };
	double s =bytes;
	int o =0, p = 2;

	while(s >= 1024 && o + 1 < bytes)
	{
		o ++;
		s =s / 1024;
	}

	if(s - floor(s) == 0.0) p = 0;
	else if(s - floor(s) <= 0.1) p = 1;

	sprintf(dsize, "%.*f %s", p, s, suffixes[o]);
	return dsize;
}

void teRecvPost(int mtype, PBMessage_t* msg, void* custom)
{
	TransfersEntry *te =(TransfersEntry*)custom;

/*	if(mtype == PB_TControl && msg->I1 == te->transfer->contact->num && 
	   msg->I2 == te->transfer->num)
	{
		dbg("Transfer %d:%d: enter new state: ", msg->I1, msg->I2);
		switch (msg->I3)
		{
		case TC_Resume:
			dbg("resume\n");
			te->progress->activate();
			te->accept->deactivate();
		}
	}
	else if(mtype == PB_TData && msg->I1 == te->transfer->contact->num && 
	   msg->I2 == te->transfer->num)
	{
		if (msg->I4)
		{
			fseek(te->transfer->file, msg->I3, SEEK_SET);
			if (fwrite(msg->S1, msg->I4, 1, te->transfer->file) != 1)
			{
				dbg("failed to write file data\n");
			}
			te->transfer->pos =msg->I3;
		}
		else
		{
			fclose(te->transfer->file);
			te->accept->deactivate();
			te->reject->deactivate();
			te->progress->value(te->transfer->size);
		}
	}
	te->redraw();*/
}

void teAcceptPressed(Fl_Widget *w)
{
	char path[255] = { 0 };
	TransfersEntry *te =(TransfersEntry*)w->parent();
	/*Event_t *e =Ev_new();

	snprintf(path, 255, "%s/%s", get_home_folder(), te->transfer->filename);
	te->transfer->file =fopen(path, "wb");

	e->T =Comm_ResumeTransfer;
	e->ID =te->transfer->num;
	e->I1 =te->transfer->contact->num;

	List_add(APP->Comm->WorkQueue, (void*)e);
	CommWork();*/
}

TransfersEntry::TransfersEntry(int X, int Y, int S, Transfer_t *T, int I)
	: Fl_Group(X, Y, parent()->w() - (224* S), 50 * S)
{
	scale =S;
	transfer =T;
	inv =I;
	strftime(date, 255, "%d/%b/%y %H:%M", transfer->time);

	fl_font(FL_HELVETICA, 11 * scale);
	dl =fl_width(date) + 20;

	accept =new Fl_Button(0, 0, 1, 1, "Accept");
	reject =new Fl_Button(0, 0, 1, 1, "Reject");
	progress =new Fl_Progress(0, 0, 1, 1);

	accept->color(fl_rgb_color(118, 202, 116));
	reject->color(fl_rgb_color(214, 78, 77));
	progress->color(fl_rgb_color(209, 210, 214));

	accept->labelsize(11 * scale);
	reject->labelsize(11 * scale);

	accept->callback(teAcceptPressed);

	progress->selection_color(fl_rgb_color(118, 202, 116));
	progress->labelcolor(fl_rgb_color(85, 85, 100));
	progress->minimum(0);
	progress->maximum(1000);
	progress->value((transfer->pos / transfer->size) * 1000);
	progress->labelsize(11.2 * scale);
	progress->labelfont(FL_HELVETICA_BOLD);
	progress->deactivate();

	box(FL_FLAT_BOX);

	if(I == 1) color(fl_rgb_color(198, 199, 214));
	else color(fl_rgb_color(239, 239, 239));

	//PB_Register(APP->events, PB_TControl | PB_TData, this, teRecvPost);

	resize(X, Y, w(), h());
	end();
}

void TransfersEntry::resize(int X, int Y, int W, int H)
{
	Fl_Group::resize(X, Y, W, H);
	accept->resize(X + w() - (78 * scale), y() + (2 * scale), 54 * scale,
	               (h() / 2) - 2 * scale);
	reject->resize(X + w() - (78 * scale), y() + (h() / 2) + (2 * scale),
	               54 * scale,(h() / 2) - 4 * scale);
	progress->resize(x() + dl, y() + (30 * scale), w() - dl - (86 * scale),
					(16 * scale));
}

void TransfersEntry::draw()
{
	int cut;
	char from[255] ={ 0 };
	char proglabel[255] ={ 0 };

	sprintf(proglabel, "%d%%", (transfer->pos / transfer->size) * 100);

	progress->value((transfer->pos / transfer->size) * 1000);
	progress->label(proglabel);
	progress->redraw();

	sprintf(from, "From: %s", GetDisplayName(transfer->contact, 100));

	Fl_Group::draw();

	fl_font(FL_HELVETICA, 11 * scale);
	fl_color(fl_rgb_color(110, 110, 118));
	fl_draw(date, x() + (10 * scale), y() + (16 * scale));
	fl_draw(from, x() + (10 * scale), y() + (30 * scale));
	fl_draw(GetDisplaySize(transfer->size), x() + (10 * scale), y() + (44 * scale));

	fl_color(fl_rgb_color(85, 85, 100));
	fl_font(FL_HELVETICA_BOLD, 12 * scale);

	for(cut =255;
	        cut > 0 &&
	        fl_width(GetShortenedText(transfer->filename, cut)) > w() - 80 -dl;
	        cut = cut - 5);

	fl_draw(GetShortenedText(transfer->filename, cut),
	        x() + dl, y() + (16 * scale));
}
