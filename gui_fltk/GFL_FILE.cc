#include "misc.h"

#include "GFL_FILE.h"
#include "GUIFLTK.h"

#define FLCB(FUNC) reinterpret_cast<FLTK_Callback_f> \
	(flThunk<GFLTransfer, decltype(&GFLTransfer::FUNC), &GFLTransfer::FUNC>),this

GFLTransfer::GFLTransfer(class GUIFLTK *gui, XWContact_t *contact,
                         const char *filename, struct tm *time, unsigned int num,
                         unsigned int size)
	: gui_(gui), contact_(contact), filename_(strdup(filename)), time_(time),
	  id_(num), size_(size), entry_(0, 0,gui->Xw_->w() - (224* gui->Xw_->scale), 50, gui->Xw_->scale, 0)
{
	fnRecvSignal =reinterpret_cast<PB_Callback_f>
	              (pbThunk<GFLTransfer, decltype(&GFLTransfer::recvSignal),
	               &GFLTransfer::recvSignal>);
	strftime(entry_.date, 255, "%d/%b/%y %H:%M", time_);
	entry_.accept->callback(FLCB(cbAccept));

	xwfSubscribe(ftControl);
	xwfSubscribe(ftBytes);
}

int GFLTransfer::xwfSubscribe(unsigned int dwType)
{
	return gui_->hObj_->pSvcs->fnSubscribe(gui_->hObj_, dwType, this,
	                                       fnRecvSignal);
}

void GFLTransfer::recvSignal(unsigned int dwType, PBMessage_t*)
{
	dbg("Signal received\n");
}

void GFLTransfer::cbAccept(Fl_Widget *w)
{
	dbg("Accept\n");
}
