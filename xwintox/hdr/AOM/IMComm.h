#ifndef __IMCOMM__H__
#define __IMCOMM__H__

#include "postbox.h"

/* The structure of an IMComm.
 * An IMComm communicates with an IM network. 
 * This is the public interface. The methods and variables
 * within are likely to be accessed from across different threads.
 * As such, it is wise to create a seperate thread 
 * (multiple, in fact) for actual IM network communication, and
 * make these public APIs message through to it. */

typedef void (*Connect_f)(void);

typedef struct IMComm_s
{
	Postbox_t *pbEvents;
	Connect_f fnConnect;

	void *pvPrivate; /* implementation-specific private storage */
} IMComm_t;

#endif
