#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>

#include "Module/Module.h"
#include "AOM/IGUICxx.h"
#include "dictionary.h"
#include "misc.h"
#include "postbox.h"

#include "GUIFLTK.h"

GUIFLTK::GUIFLTK(XWF_ObjectParams_t *)
{
	dbg("Initialising a new GUI for FLTK\n");
}

int GUIFLTK::start()
{
	dbg("Although I am a C++ object, implementing what appears to me to be a standard interface class-template, I have been called from C, having been dynamically instantiated, using the native C object API, with no changes.\n");
	return 0;
}
