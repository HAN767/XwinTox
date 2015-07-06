#ifndef __MODULE_H__
#define __MODULE_H__

/* Which language is the module written in?
 * This enumerates the types possible. */
typedef enum XWF_Lang
{
	XWF_Lang_C,
	XWF_Lang_CXX,
	XWF_Lang_Script,
} XWF_Lang_e;

struct XWF_Services_s;

/* These are the parameters passed to an object's create() call.
 * The name of the object type whose creation is wanted, and a
 * handle to the XWFramework's services provision structure. */ 
typedef struct XWF_ObjectParams_s
{
	const char *szObjType;
	const struct XWF_Services *hsrvServices;
} XWF_ObjectParams_t;

/* The create and destroy function types for an object. */
typedef void *(*XWF_Create_f)(struct XWF_Services_s *);
typedef int (*XWF_Destroy_f)(void *);

/* The parameters a module must pass while registering with
 * the module manager.
 * Note: uiVersion refers to the module manager version. */
typedef struct XWF_RegisterParams_s
{
	unsigned int uiVersion;
	XWF_Create_f fnCreate;
	XWF_Destroy_f fnDestroy;
	XWF_Lang_e enLang;
} XWF_RegisterParams_t;

/* This function allows a module to register an object with
 * the module manager. It should name either an object it
 * can create, or can pass '*' - this will cause fnCreate to
 * be called for any attempted object creation, regardless of
 * type, by the module manager. */
typedef int (*XWF_RegisterObj_f)(const char *szType, 
							  const XWF_RegisterParams_t *hrpParams);
/* This function allows a module to call either a system service,
 * or a service provided by another object. 
 * The syntax is SYSTEM/ServiceName for system services, and
 * ObjType/ServiceName for other objects. */
typedef int (*XWF_Call_f)(const char *szService, void *hvParams);

/* This structure provides modules with services from the 
 * module manager. */
typedef struct XWF_Services_s
{
	unsigned int uiVersion;
	XWF_RegisterObj_f fnRegisterObj;
	XWF_Call_f fnCall;
} XWF_Services_t;

/* The function type of a module's exit function. */
typedef int (*XWF_Exit_f)();

/* The function type of a module's initialisation function.
 * It returns either a pointer to the module's exit function,
 * or 0 to indicate failure. */
typedef XWF_Exit_f (*XWF_Init_f)(const XWF_Services_t*);


#endif
