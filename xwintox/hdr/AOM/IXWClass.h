#ifndef __IXWCLASS__H__
#define __IXWCLASS__H__

#ifdef __cplusplus
extern "C"
{
#endif

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
}
#endif


#ifdef __cplusplus

#include <vector>


template<class T, class Method, Method m>
static void pbThunk(unsigned int dwType,PBMessage_t *msg, void *userData)
{
	return ((*static_cast<T *>(userData)).*m)(dwType, msg);
}

template <typename T>
class XWClassT
{
public:
	XWClassT()
	{
		cXWClass_.hCXXObj =this;
		cXWClass_.fnStart =staticStart;
		fnRecvSignal =reinterpret_cast<PB_Callback_f>
		              (pbThunk<T, decltype(&T::recvSignal), &T::recvSignal>);
	}
	~XWClassT()
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
		getSelf(pguiSelf)->hObj_ =pguiSelf;
		return getSelf(pguiSelf)->start();
	}

	void *xwfCall(const char *pszService, void *pvParams)
	{
		return hObj_->pSvcs->fnCall(hObj_, pszService, pvParams);
	}

	const char *pszxwfCall(const char *pszService, const char *pvParams)
	{
		return static_cast<const char*>
		       (xwfCall(pszService, const_cast<char*>(pvParams)));
	}

	int xwfSubscribe(unsigned int dwType)
	{
		return hObj_->pSvcs->fnSubscribe(hObj_, dwType, this, fnRecvSignal);
	}

	int xwfDispatch(unsigned int dwType, PBMessage_t *pbmMsg)
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

	void recvSignal(unsigned int dwType, PBMessage_t*)
	{
		dbg("Signal received: %d\n");
	}


	int start()
	{
		return 0;
	}

	const XWF_Object_Handle_t *hObj_;

protected:
	PB_Callback_f fnRecvSignal;
	XWClass_t cXWClass_;
};

#endif

#endif
