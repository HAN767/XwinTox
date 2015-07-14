#ifndef __IGUICXX__H__
#define __IGUICXX__H__

#include "Module/Module.h"
#include "IGUI.h"

#include "misc.h"

template <typename T>
class GUICxxInterfaceT
{
public:
	GUICxxInterfaceT()// : pSvcs(NULL)
	{
		hCGUI_.hCXXObj =this;
		hCGUI_.fnStart =staticStart;
	}
	virtual ~GUICxxInterfaceT()
	{
	}
	static GUICxxInterfaceT *getSelf(IGUI_t *pguiSelf)
	{
		return static_cast<GUICxxInterfaceT *> (pguiSelf->hCXXObj);
	}
	static GUICxxInterfaceT *getSelf(XWF_Object_Handle_t *pguiSelf)
	{
		return static_cast<GUICxxInterfaceT *> 
		(((IGUI_t*)pguiSelf->hObj)->hCXXObj);
	}
	static void * create(XWF_ObjectParams_t *pobpParams)
	{
		T *GUI =new T(pobpParams);
		return &GUI->hCGUI_;
	}
	static int destroy(void *pobjToDestroy)
	{
		IGUI_t *pguiToDestroy =(IGUI_t*) pobjToDestroy;
		if(!pguiToDestroy)
			return -1;

		delete((T*)pguiToDestroy->hCXXObj);
		delete pguiToDestroy;
		return 0;
	}
	static int staticStart(XWF_Object_Handle_t *pguiSelf)
	{
		return getSelf(pguiSelf)->start();
	}

	virtual int start()
	{
		return 0;
	}

	const XWF_Object_Handle_t *hObj_;

protected:
	IGUI_t hCGUI_;
};

#endif
