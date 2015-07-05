/*
 * LibSunshine
 *
 * Miscellaneous utility functionality
 */

#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>

#define DEF_EV
#include "misc.h"

/* Debugging functionality */
void
dbg2(const char *funcname, const char *format,  ...)
{
	va_list arglist;
	va_start(arglist, format);
	fprintf(stderr, KRED "[dbg] " KMAG "[%s] " KNRM, funcname);
	vfprintf(stderr, format, arglist);
	va_end(arglist);
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
	free(ev);
	return;
}
