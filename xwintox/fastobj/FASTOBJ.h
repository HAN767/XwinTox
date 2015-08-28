#ifndef __FASTOBJ__H__
#define __FASTOBJ__H__

#include <threads.h>

#include "misc.h"
#include "xwfvm.h"

#include "Module/Module.h"
#include "AOM/IXWClass.h"

extern Context context;

class FASTOBJ : public XWClassT< FASTOBJ >
{
  public:
    FASTOBJ (XWF_ObjectParams_t * pobjParams);
    int start () { return 0; }
    VM Vm;

  private:
    static int vmThread (void * customData);
    thrd_t thrdInteractive;
};

#endif
