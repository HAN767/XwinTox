/*
 * LibSunshine
 *
 * Miscellaneous utility functionality
 */

#include <errno.h>
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <pwd.h>
#include <unistd.h>

#include <threads.h>

#define DEF_EV
#include "misc.h"

/* Debugging functionality */
void
dbg2(const char *funcname, const char *format,  ...)
{
	/* put it in a buffer or something. */
}

/* Events type manipulation */
Event_t *Ev_new()
{
	return (Event_t*)calloc(1, sizeof(Event_t));
}

Event_t *Ev_copy(Event_t *ev)
{
	return ev;
}

void Ev_pack(Event_t *ev)
{
	if(!ev->S1) ev->S1 =malloc(4);

	if(!ev->S2) ev->S2 =malloc(4);
}

void Ev_free(Event_t *ev)
{
	free(ev->S1);
	free(ev->S2);
	if(ev->O.O_len) free (ev->O.O_val);
	free(ev);
	return;
}

/* Files */
int create_folder_if_not_exist(const char *path)
{
	int error;

	if((error =mkdir(path, 0700)) == -1)
	{
		struct stat s;

		if(errno == EEXIST && ! stat(path, &s) &&
		        s.st_mode & S_IFDIR)
		{
			return 0;
		}
		else
		{
			dbg("Failed to create config folder: %s", strerror(errno));
			return 1;
		}
	}

	return 0;
}

const char *get_home_folder()
{
	const char *homefolder;

	if((homefolder =getenv("HOME")) == NULL)
	{
		homefolder =getpwuid(getuid())->pw_dir;
	}

	return homefolder;
}
