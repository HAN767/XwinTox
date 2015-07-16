#ifndef __IGUICXX__H__
#define __IGUICXX__H__

#include <vector>

#include "list.h"
#include "misc.h"

#include "Module/Module.h"

typedef int (*Start_f)(XWF_Object_Handle_t *);

/* The interface for a module.
 * This is the framework-public interface. The methods and variables
 * within are likely to be accessed from across different threads.
 * The implementation, then, should have a main loop executing in
 * a different thread, and either deliver messages to that thread
 * or use locking.
 * That main loop will be started by fnStart called for the module.
 * If you don't need a main loop, just blank it (or, in C++, leave 
 * void start as is) */
typedef struct XWClass_s
{
	Start_f fnStart;

	void *pvPrivate; /* implementation-specific private storage.
					  * you probably only need it for C implementations */
	void *hCXXObj; /* used for C++ wrapper */
} XWClass_t;

#ifdef __cplusplus

template <typename T>
class XWClassT
{
public:
	XWClassT()// : pSvcs(NULL)
	{
		cXWClass_.hCXXObj =this;
		cXWClass_.fnStart =staticStart;
	}
	virtual ~XWClassT()
	{
	}
	static XWClassT *getSelf(XWClass_t *pguiSelf)
	{
		return static_cast<XWClassT *>(pguiSelf->hCXXObj);
	}
	static XWClassT *getSelf(XWF_Object_Handle_t *pguiSelf)
	{
		return static_cast<XWClassT *>
		       (((XWClass_t *)pguiSelf->hObj)->hCXXObj);
	}
	static void * create(XWF_ObjectParams_t *pobpParams)
	{
		T *NewObject =new T(pobpParams);
		return &NewObject->cXWClass_;
	}
	static int destroy(void *pvToDestroy)
	{
		XWClass_t *pobjToDestroy =static_cast<XWClass_t *>(pvToDestroy);

		if(!pobjToDestroy)
			return -1;

		delete((T *)pobjToDestroy->hCXXObj);
		delete pobjToDestroy;
		return 0;
	}
	static int staticStart(XWF_Object_Handle_t *pguiSelf)
	{
		return getSelf(pguiSelf)->start();
	}

	void *xwfCall(const char *pszService, void *pvParams)
	{
		return hObj_->pSvcs->fnCall(hObj_, pszService, pvParams);
	}

	void *xwfSubscribe(unsigned int dwType, void *pvCustom, PB_Callback_f fnCallback)
	{
		return hObj_->pSvcs->fnSubscribe(hObj_, dwType, pvCustom, fnCallback);
	}

	void *xwfDispatch(unsigned int dwType, PBMessage_t *pbmMsg)
	{
		return hObj_->pSvcs->fnDispatch(hObj_, dwType, pbmMsg);
	}

	std::vector<XWF_ModInfo_t *> xwfGetModulesInfo()
	{
		List_t *lstModinfos =xwfCall("/GETMODULESINFO", 0);
		std::vector<XWF_ModInfo_t *> vecModinfos;

		LIST_ITERATE_OPEN(lstModinfos)
			vecModinfos.push_back(static_cast<XWF_ModInfo_t *>(e_data));
		LIST_ITERATE_CLOSE(lstModinfos)

		return vecModinfos;
	}

	virtual int start()
	{
		return 0;
	}

protected:
	const XWF_Object_Handle_t *hObj_;
	XWClass_t cXWClass_;
};

#endif

#endif
