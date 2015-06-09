/*
 * LibSunshine
 *
 * Miscellaneous utility functionality
 */

#include <stdio.h>
#include <stdarg.h>

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

