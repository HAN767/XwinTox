#ifndef __IGUI__H__
#define __IGUI__H__

#include "Module/Module.h"

struct IGUI_s;
typedef int (*Start_f)(XWF_Object_Handle_t *);

/* The interface for a GUI.
 * An MComm communicates with an IM network.
 * This is the framework-public interface. The methods and variables
 * within are likely to be accessed from across different threads.
 * The implementation, then, should have a main loop executing in
 * a different thread, and either deliver messages to that thread
 * or use locking. */
typedef struct IGUI_s
{
	Start_f fnStart;

	void *pvPrivate; /* implementation-specific private storage */
	void *hCXXObj; /* used for C++ wrapper */
} IGUI_t;

#define GUIOBJ(X) ((IGUI_t*) X->hObj)

#endif
