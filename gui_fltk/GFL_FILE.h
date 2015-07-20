#ifndef _GFL_FILE_H
#define _GFL_FILE_H

#include "xwintox.h"
#include "control/transent.h"

class GFLTransfer
{
	friend class GUIFLTK;
	friend class TransfersEntry;

public:
	GFLTransfer(class GUIFLTK *owner, XWContact_t *contact, const char *filename,
	            struct tm *time, unsigned int num, unsigned int size, 
				unsigned int dir);

	void cbAccept(Fl_Widget *w);
	void recvSignal(unsigned int dwType, PBMessage_t*);


	char *localfilename_;
protected:
	const char *filename_;
	struct tm *time_;
	unsigned int id_, size_, pos_, dir_;
	XWContact_t *contact_;
	FILE* file_;

private:
	class GUIFLTK *gui_;
	class TransfersEntry *entry_;
	PB_Callback_f fnRecvSignal_;

	int xwfSubscribe_(unsigned int dwType);
};

#endif
