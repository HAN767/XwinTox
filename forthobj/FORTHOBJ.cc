#include <cstdio>
#include <cstring>
#include <sys/stat.h>
#include <threads.h>
#include <unistd.h>

#include "misc.h"
#include "hexstring.h"

#include "xwintox.h"
#include "AOM/IXWClass.h"
#include "Module/Module.h"
#include "FORTHOBJ.h"

FORTHOBJ::FORTHOBJ(XWF_ObjectParams_t *prmObj)
{
	printf("New FORTHOBJ created. Subtype: %s\n", prmObj->pszObjSubtype);
	instances.push_back(this);
	vm_ =ficlSystemCreateVm(globalSystem);
	ficlVmEvaluate (vm_, ".( XwinTox Objective-Forth " __DATE__ " ) cr quit") ;
	if(prmObj->pszObjSubtype != 0 && (strcmp(prmObj->pszObjSubtype, "INTER") == 0))
	{
		thrd_create(&thrdInteractive, vmThread, this);
	}
}

int FORTHOBJ::start()
{
	return 0;
}

int FORTHOBJ::vmThread(void *customData)
{
	FORTHOBJ *self =static_cast<FORTHOBJ*>(customData);
    char buffer[256];
	int returnValue =0;
	ficlVmEvaluate (self->vm_, 
					".( XwinTox Command Interpreter ) cr "
					".( This is an interactive Objective-Forth environment. ) cr"
					" .( Full access is provided to the XwinTox object model. )" 
					" cr quit");
    while (returnValue != FICL_VM_STATUS_USER_EXIT) 
	{
        fputs("ok ", stdout);
        fflush(stdout);

        if (fgets(buffer, sizeof(buffer), stdin) == NULL) break;
        returnValue = ficlVmEvaluate(self->vm_, buffer);
    }
	return 0;
}

void FORTHOBJ::wordXWFSUBSCRIBE(ficlVm *vm) /* ( EventID -- ) */
{
	ficlCell cell;

	FICL_STACK_CHECK(vm->dataStack, 1, 0);

	cell =ficlVmPop(vm);

	for(FORTHOBJ* fObj : instances)
	{
		printf("Subscribe to signal %lu\n", cell.u);
		if(fObj->vm_ == vm) fObj->xwfSubscribe(cell.u);
		return;
	}
}

void FORTHOBJ::deliverString(ficlVm *vm, const char *string)
{
	ficlCell cell;

	if (!string)
	{
		cell.p =0;
		ficlVmPush(vm, cell);
		cell.i =0;
		ficlVmPush(vm, cell);
	}
	else
	{
		char *stringVm =strdup(string);

		cell.p =stringVm;
		ficlVmPush(vm, cell);
		cell.u =strlen(stringVm);
		ficlVmPush(vm, cell);
	}
}

void FORTHOBJ::deliverPBMessage(unsigned int mtype, PBMessage_t *msg)
{
	ficlCell cell;

	cell.u =mtype;
	ficlVmPush(vm_, cell);

	cell.i =msg->I1;
	ficlVmPush(vm_, cell);
	cell.i =msg->I2;
	ficlVmPush(vm_, cell);
	cell.i =msg->I3;
	ficlVmPush(vm_, cell);
	cell.i =msg->I4;
	ficlVmPush(vm_, cell);

	deliverString(vm_, msg->S1);
	deliverString(vm_, msg->S2);
	deliverString(vm_, msg->S3);
	deliverString(vm_, msg->S4);
}

