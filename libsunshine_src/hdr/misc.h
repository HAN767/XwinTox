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


#define dbg(format,args...)        \
	dbg2(__func__, format, ## args);

typedef struct Call_s
{
	unsigned long Func;
	char *S1, *S2;
	int I1, I2;
} Call_t;

void dbg2(const char *funcname, const char *format,  ...);
typedef struct Event_s Event_t;
Event_t *Ev_copy(Event_t *ev);
void Ev_pack(Event_t *ev);
void Ev_free(Event_t *ev);

#ifdef DEF_EV

typedef unsigned int u_int;

typedef struct Event_s {
	int T;
	int ID;
	char *S1;
	char *S2;
	int I1;
	int I2;
	struct {
		u_int SI1_len;
		short *SI1_val;
	} SI1;
	struct {
		u_int SI2_len;
		short *SI2_val;
	} SI2;
	struct {
		u_int O_len;
		char *O_val;
	} O;
} Event_t;
#endif

#ifdef __cplusplus
}
#endif

#endif

