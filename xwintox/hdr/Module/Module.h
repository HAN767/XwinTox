#ifndef __MODULE_H__
#define __MODULE_H__

#ifdef __cplusplus
extern "C"
{
#endif

#include "postbox.h"

/* Which language is the module written in?
 * This enumerates the types possible. */
typedef enum XWF_Lang
{
	XWF_Lang_C,
	XWF_Lang_CXX,
	XWF_Lang_Script,
}
XWF_Lang_e;

typedef enum XWF_Modtype
{
    XWF_Dynamic,
    XWF_Static,
} XWF_Modtype_e;

/* The function type of a module's exit function. */
typedef int (*XWF_Exit_f)();

/* A structure detailing a module. It contains a null-terminated
 * string containing the module's name, and a pointer to the
 * module's exit function. */
typedef struct XWF_Module_s
{
	const char *pszName;
	XWF_Exit_f fnExit;
	XWF_Modtype_e enModtype;
	void *hdlLib;
} XWF_Module_t;

struct XWF_Services_s;
struct XWF_Object_Handle_s;

/* These are the parameters passed to an object's create() call.
 * The name of the object type whose creation is wanted, and a
 * pointer to the XWFramework's services provision structure. */
typedef struct XWF_ObjectParams_s
{
	const char *pszObjType;
	const struct XWF_Object_Handle_s *pobjhHandle;
	const struct XWF_Services_s *psrvServices;
} XWF_ObjectParams_t;

/* The create and destroy function types for an object. */
typedef void *(*XWF_Create_f)(XWF_ObjectParams_t *);
typedef int (*XWF_Destroy_f)(void *);

struct XWF_Services_s;

/* The parameters a module must pass while registering with
 * the module manager.
 * Note: uiVersion refers to the module manager version that
 * the object was written for. */
typedef struct XWF_Class_s
{
	const char *pszType;
	const char *pszSubtype;
	unsigned int uiVersion;
	XWF_Create_f fnCreate;
	XWF_Destroy_f fnDestroy;
	XWF_Lang_e enLang;
	XWF_Module_t *pmodProvider;
} XWF_Class_t;

/* This is a handle to an object.
 * It includes a pointer to the XWF Object for use by the framework. */
typedef struct XWF_Object_Handle_s
{
	XWF_Class_t *pxwoClass;
	void *hObj;
	const struct XWF_Services_s *pSvcs;
} XWF_Object_Handle_t;

#define XWF_hObj_t XWF_Object_Handle_t /*short version for cleanliness*/

/* This function allows a module to register an object with
 * the module manager. It should name either an object it
 * can create, or can pass '*' - this will cause fnCreate to
 * be called for any attempted object creation, regardless of
 * type, by the module manager. */
typedef int (*XWF_RegisterClass_f)(const XWF_Class_t *pobjRegistered);
/* This function allows a module to call either a system service,
 * or a service provided by another object.
 * The syntax is SYSTEM/ServiceName for system services, and
 * ObjType/ServiceName for other objects. */
typedef void *(*XWF_Call_f)(const XWF_Object_Handle_t *, const char *,
                            const void *);
/* This function allows a module to subscribe to a message type.
 * It may pass a custom parameter if desired.
 * 0 is returned if the module is permitted to do so, and
 * 1 is returned if the module is not permitted to. */
typedef int (*XWF_Subscribe_f)(const XWF_Object_Handle_t *, int mtype,
                              void *custom, PB_Callback_f);
/* This function allows a module to despatch a message.
 * 0 is returned if the module is permitted to do so, and
 * 1 is returned if the module is not permitted to. */
typedef int (*XWF_Dispatch_f)(const XWF_Object_Handle_t *, int, PBMessage_t *);

/* This structure provides modules with services from the
 * module manager. */
typedef struct XWF_Services_s
{
	unsigned int uiVersion;
	XWF_RegisterClass_f fnRegisterClass;
	XWF_Call_f fnCall;

	XWF_Subscribe_f fnSubscribe;
	XWF_Dispatch_f fnDispatch;
} XWF_Services_t;

/* The function type of a module's initialisation function.
 * It returns either 0 for success, 1 for non-critical failure,
 * or 2 for critical failure. A critical failure leads to an
 * unloading of the module, so delete all resources held first. */
typedef int (*XWF_Init_f)(XWF_Module_t*, const XWF_Services_t*);

#ifdef __cplusplus
}
#endif

#endif
