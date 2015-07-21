#include <sys/stat.h>
#include <FL/Fl_Multiline_Input.H>
#include <FL/Fl_Native_File_Chooser.H>

#include "nanosvg/svgs.h"
#include "xwintox.h"

#include "control/xwin.h"
#include "control/msgarea.h"
#include "control/svgbox.h"
#include "misc.h"
#include "util.h"

void mareaSendfilePressed(Fl_Widget* B , void*)
{
	PBMessage_t *msgSendfile =PB_New_Message();
	GMessageArea *cArea =((GMessageArea*)B->parent());
	Fl_Native_File_Chooser fnfc;

	fnfc.title("Select a File");
	fnfc.type(Fl_Native_File_Chooser::BROWSE_FILE);
	int res =fnfc.show();
	if (res == 1 | res == -1) goto cleanup;
	else
	{
		struct stat statFile;
		char *const pszFilename =strdup(fnfc.filename());
		if (stat(pszFilename, &statFile) == -1)
		{
			free(pszFilename);
			goto cleanup;
		}
		msgSendfile->I1 =cArea->contact->wNum;
		msgSendfile->I2 =statFile.st_size;
		msgSendfile->S1 =pszFilename;
		cArea->hObj_->pSvcs->fnDispatch(cArea->hObj_, ftSend, msgSendfile);
	}

	return;

cleanup:
	free(msgSendfile);
}

void mareaFrMsg(int, PBMessage_t *msg, void *custom)
{
	GMessageArea *self =static_cast<GMessageArea*>(custom);
	char nmsg[1168];

	if((msg->I1) != self->contact->wNum) return;

	Fl::lock();
	snprintf(nmsg, 1168, "%s: %s\n", self->contact->pszName, msg->S1);
	self->moutbuffer->append(nmsg);
	self->moutput->scroll(self->moutput->count_lines(0,
	                      self->moutbuffer->length(), 1), 0);
	self->redraw();
	Fl::unlock();
	Fl::awake();
}

void mareaSendPressed(Fl_Widget* B , void*)
{
	PBMessage_t *msgNew =PB_New_Message();
	char nmsg[1024];
	GMessageArea *cArea =((GMessageArea*)B->parent());
	const char *msg =cArea->message->value();
	XwinTox *Xw =getXwin(B);

	snprintf(nmsg, 1024, "%s: %s\n", Xw->sidebar->top_area->name->value(),
	         ((GMessageArea*)B->parent())->message->value());

	if(!((GMessageArea*)B->parent())->mtype) /* contact */
	{
		unsigned wNum =cArea->contact->wNum;
		msgNew->I1 =wNum;
		msgNew->S1 =strdup(msg);
		cArea->hObj_->pSvcs->fnDispatch(cArea->hObj_, frSendMsg, msgNew);

		cArea->moutbuffer->append(nmsg);
		cArea->message->value("");
		cArea->message->take_focus();
		cArea->moutput->scroll
		(cArea->moutput->count_lines(0,
		                             cArea->moutbuffer->length(), 1), 0);
	}
	else
	{
		free(msgNew);
	}
}

GMessageArea::GMessageArea(const XWF_hObj_t* hObj, int S, XWContact_t *C,
                           XWGroupchat_t *G, short T) : Fl_Group(0, 0, 1, 1)
{
	hObj_ =hObj;
	contact =C;
	groupchat =G;
	scale =S;
	mtype =T;

	box(FL_FLAT_BOX);
	color(255);

	if(!T)
	{
		icon = new SVGBox(0, 0,  40 * scale,  40 * scale, S, default_av, 1);
		icon->show();
		groupchat =new XWGroupchat_t;
		groupchat->wNum =65535;
		hObj_->pSvcs->fnSubscribe(hObj_, frMsg, this, mareaFrMsg);
	}
	else
	{
		icon = new SVGBox(0, 0,  40 * scale,  40 * scale, S, groupsvg2, 1);
		icon->show();
		contact =new XWContact_t;
		contact->wNum =65535;
	}

	names =new Fl_Select_Browser(0, 0, 60 * scale, h() - 80 * scale);
	gnames =new Fl_Multiline_Output(0,0, w() - (65 * scale), (28 * scale));
	moutput =new Fl_Text_Display(0, 0, 0, 0);
	moutbuffer =new Fl_Text_Buffer();
	send =new Fl_Button(0, 0, (60 * scale), (74 * scale) , "Send");
	message =new Fl_Multiline_Input(0, 0, (w() - (110 * S)), (74 * S));
	attach =new SVGBox(0, 0, 37 * scale, 37 * scale, scale, attachsvg, 1);
	emoji =new SVGBox(0, 0, 37 * scale, 37 * scale, scale, emojisvg, 1);

	icon->box(FL_ENGRAVED_BOX);
	icon->color(fl_rgb_color(130, 128, 133));

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
	send->callback(mareaSendPressed, this);

	moutput->wrap_mode(moutput->WRAP_AT_BOUNDS, 0);
	moutput->box(FL_NO_BOX);
	moutput->textsize(12 * S);
	moutput->buffer(moutbuffer);

	attach->color(fl_rgb_color(107, 194, 96));
	attach->box(FL_BORDER_BOX);
	attach->show();
	attach->callback(mareaSendfilePressed, this);

	emoji->color(fl_rgb_color(107, 194, 96));
	emoji->box(FL_BORDER_BOX);
	emoji->show();

	resize(0, 0, 0, 0);
	end();
}

void GMessageArea::resize(int X, int Y, int W, int H)
{
	Fl_Group::resize(X, Y, W, H);


	icon->resize(X + (12 * scale), Y + (9 * scale), 40 * scale, 40 * scale);

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

	send->resize(x() + (w() - 64 * scale), y() + (h() - (80 * scale)),
	             (60 * scale), (74 * scale));
	message->resize(x() + (5 * scale), y() + h() - (80 * scale),
	                (w() - (110 * scale)), (74 * scale));
	emoji->resize(x() + (w() - (105 * scale)), y() + h() - (80 * scale),
	              37 * scale, 38 * scale);
	attach->resize(x() + (w() - (105 * scale)), y() + h() - (43 * scale),
	               37 * scale, 37 * scale);


}

void GMessageArea::draw()
{
	fl_push_clip(x(), y(), w(), h());

	if(mtype)
	{
		if(strcmp(gnames->value(), *groupchat->arpszPeers))
		{
			gnames->value(*groupchat->arpszPeers);
			gnames->redraw();
		}
	}

	Fl_Group::draw();
	emoji->show();
	attach->show();

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
		/*char tmp[128];
		sprintf(tmp, "%d peers in chat", groupchat->num_peers);
		fl_font(FL_HELVETICA_BOLD, 12 * scale);
		fl_draw(groupchat->name, x() + (60 * scale), y() + (26 * scale));
		fl_font(FL_HELVETICA_ITALIC, 11 * scale);
		fl_color(fl_rgb_color(56, 56, 56));
		fl_draw(tmp, x() + (60 * scale), y() + (38 * scale));
		fl_color(fl_rgb_color(192, 192, 192));
		fl_line(x() + w() - (95 * scale), y() + (60 * scale),
		        x() + w() - (95 * scale),y() + h() - ((85)  * scale));*/
	}


	fl_color(fl_rgb_color(192, 192, 192));
	fl_line(x(), y() + (60 * scale), Fl::w(), y() + (60 * scale));
	fl_line(x(), h() + y() - (85 * scale), Fl::w(), h() + y() -
	        (85 * scale));

	fl_pop_clip();
}
