/*
 * LibSunshine
 *
 * Miscellaneous utility functionality
 */

#ifndef MISC_H_
#define MISC_H_

/* threading related */
#include <threads.h>

#define _Locktype_ mtx_t

#define _Lock_ mtx_t _Lock;

#define _Lock_Init(X) mtx_init(&X, mtx_plain);

#define _Locked_Start(X) mtx_lock(&X); 
#define _Locked_End(X) mtx_unlock(&X);

#define KNRM  "\x1B[0m"
#define KRED  "\x1B[31m"
#define KGRN  "\x1B[32m"
#define KYEL  "\x1B[33m"
#define KBLU  "\x1B[34m"
#define KMAG  "\x1B[35m"
#define KCYN  "\x1B[36m"
#define KWHT  "\x1B[37m"


#define dbg(format,args...)        \
	dbg2(__func__, format, ## args);

void dbg2(const char *funcname, const char *format,  ...);

#endif

