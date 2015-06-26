/*
 * LibSunshine
 *
 * Miscellaneous utility functionality
 */

#ifndef MISC_H_
#define MISC_H_

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

typedef struct Call_s
{
	unsigned long Func;
	char *S1, *S2;
	int I1, I2;
} Call_t;

void dbg2(const char *funcname, const char *format,  ...);

#endif

