#ifndef __IMCOMM__H__
#define __IMCOMM__H__

#include "dictionary.h"
#include "postbox.h"

struct IMComm_s;
typedef int (*Connect_f)(struct IMComm_s *);

/* The structure of an MComm.
 * An MComm communicates with an IM network.
 * This is the public interface. The methods and variables
 * within are likely to be accessed from across different threads.
 * As such, it is wise to create a seperate thread 
 * (multiple, in fact) for actual IM network communication, and
 * make these public APIs message through to it. */
typedef struct IMComm_s
{
	Postbox_t *pbEvents;
	Connect_f fnConnect;

	char *pszName;
	char *pszStatus;

	char szConfigFile[255]; /* implementation-specific configuration file */
	Dictionary_t *dictConfig; /* store implementation-specific config here */
	void *pvPrivate; /* implementation-specific private storage */
} IMComm_t;

#endif
