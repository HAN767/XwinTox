#include "misc.h"

#include "GUIFLTK.h"
#include "util.h"

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

	/* try to identify what type of request this is */
	if((strlen(id) == 76) && !strchr(id, '@'))
	{
		PBMessage_t *msgAddFriend =PB_New_Message();

		dbg("Regular Tox key\n");

		msgAddFriend->S1 =id;
		msgAddFriend->S2 =msg;

		xwfDispatch(frSendRequest, msgAddFriend);

		g->id->value("");
		return;
	}
	else if(strchr(id, '@'))
	{
		dbg("ToxDNS ID\n");
		/*	Call_t *call =(Call_t*)calloc(1, sizeof(Call_t));
			call->Func =R_DNSResolve;
			call->S1 =id;
			List_add(APP->Resolv->Calls, call);
			ResolvAddWork(1);*/
		free(id); free(msg);
		return;
	}
	else
	{
		dbg("ID unrecognised\n");
		free(id); free(msg);
	}
}
