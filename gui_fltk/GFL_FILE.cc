#include "misc.h"

#include "GFL_FILE.h"
#include "GUIFLTK.h"

#define FLCB(FUNC) reinterpret_cast<FLTK_Callback_f> \
	(flThunk<GFLTransfer, decltype(&GFLTransfer::FUNC), &GFLTransfer::FUNC>),this

GFLTransfer::GFLTransfer(class GUIFLTK *gui, XWContact_t *pcontact,
                         const char *filename, struct tm *time, unsigned int num,
                         unsigned int size, unsigned int dir)
	: contact_(pcontact), gui_(gui), filename_(strdup(filename)), time_(time),
	  id_(num), size_(size), pos_(0), dir_(dir)
{
	dbg("Pos: %d. Size: %d\n", pos_, size_);
	asprintf(&localfilename_, "%s/%s", gui_->xwfCall("APP/GetHomeFolder", 0),
 filename_);
	fnRecvSignal_ =reinterpret_cast<PB_Callback_f>
	               (pbThunk<GFLTransfer, decltype(&GFLTransfer::recvSignal),
	                &GFLTransfer::recvSignal>);
	entry_ =new TransfersEntry(0, 0,gui->Xw_->w() - (224* gui->Xw_->scale), 50,
	                           gui->Xw_->scale, time, this, 0);
	entry_->accept->callback(FLCB(cbAccept));

	xwfSubscribe_(ftControl);
	xwfSubscribe_(ftBytes);
}

int GFLTransfer::xwfSubscribe_(unsigned int dwType)
{
	return gui_->hObj_->pSvcs->fnSubscribe(gui_->hObj_, dwType, this,
	                                       fnRecvSignal_);
}

void GFLTransfer::recvSignal(unsigned int dwType, PBMessage_t*)
{
	dbg("Signal received\n");
}

void GFLTransfer::cbAccept(Fl_Widget *w)
{
	dbg("Accept\n");
	//entry_->accept->deactivate();
	//entry_->progress->activate();
	
}
