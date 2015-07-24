#ifndef __GUIFLTK__H
#define __GUIFLTK__H

#include <threads.h>

#include "misc.h"
#include "ficl.h"

#include "Module/Module.h"
#include "AOM/IXWClass.h"

class FORTHOBJ : public XWClassT<FORTHOBJ>
{
public:
	FORTHOBJ(XWF_ObjectParams_t *pobjParams);
	int start();
	void recvSignal(unsigned int, PBMessage_t*);

	ficlVm *vm_;
	ficlWord *xtSignal;
	void *userData;

	static void deliverString(ficlVm *vm, const char *string);
	static void wordXWFSUBSCRIBE(ficlVm *vm);
	static void wordXWFSETCALLBACK(ficlVm *vm);

private:
	static int vmThread(void *customData);
	void deliverPBMessage(unsigned int mtype, PBMessage_t *msg);
	thrd_t thrdInteractive;
};

extern ficlSystem *globalSystem;
extern std::vector<FORTHOBJ*> instances;

#endif
