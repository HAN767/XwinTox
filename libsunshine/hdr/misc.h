/*
 * LibSunshine
 *
 * Miscellaneous utility functionality
 */

#ifndef MISC_H_
#define MISC_H_

#ifdef __cplusplus
extern "C"
{
#endif

#define KNRM  "\x1B[0m"
#define KRED  "\x1B[31m"
#define KGRN  "\x1B[32m"
#define KYEL  "\x1B[33m"
#define KBLU  "\x1B[34m"
#define KMAG  "\x1B[35m"
#define KCYN  "\x1B[36m"
#define KWHT  "\x1B[37m"

#if defined(__GNUC__) || defined(__GNUG__) || defined(__clang__)
#define PRINTFUNCNAME __PRETTY_FUNCTION__
#else
#define PRINTFUNCNAME __func__
#endif


#define dbg(format,args...)        \
	dbg2(PRINTFUNCNAME, format, ## args);

typedef struct Bytes_s
{
	char *pbData;
	unsigned int wLength;
} Bytes_t;

void dbg2(const char *funcname, const char *format,  ...);
void dbgPrint();

int create_folder_if_not_exist(const char *path);
const char *get_home_folder();


#ifdef __cplusplus
}
#endif

#endif

