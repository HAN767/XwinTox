#include "misc.h"

#include "GUIFLTK.h"
#include "util.h"

#define FLCB(FUNC) reinterpret_cast<FLTK_Callback_f> \
	(flThunk<GUIFLTK, decltype(&GUIFLTK::FUNC), &GUIFLTK::FUNC>), this

void GUIFLTK::setFLTKCallbacks()
{
	Xw_->contents->addfriend->send->callback(FLCB(cbAddFriendSend));
}

void GUIFLTK::cbAddFriendSend(Fl_Widget *w)
{
	GAddFriend *g =(GAddFriend*)w->parent();
	char *id =strdup(g->id->value());
	char *msg =strdup(g->message->value());

	RemoveSpaces(id);

	PBMessage_t *msgAddFriend =PB_New_Message();

	dbg("Regular Tox key\n");
	msgAddFriend->P =PB_Slow;
	msgAddFriend->S1 =id;
	msgAddFriend->S2 =msg;

	xwfDispatch(frSendRequest, msgAddFriend);

	g->id->value("");
	return;
}
