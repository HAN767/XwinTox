#ifndef __IGUICXX__H__
#define __IGUICXX__H__

#include "Module/Module.h"
#include "IGUI.h"

template <typename T>
class GUICxxInterfaceT
{
public:
	GUICxxInterfaceT() : fnCall_(NULL)
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
		(((IGUI_t*)pguiSelf->pobjObject)->hCXXObj);
	}
	static void * create(XWF_ObjectParams_t *pobpParams)
	{
		T *GUI =new T(pobpParams);
		GUI->fnCall_ =pobpParams->psrvServices->fnCall;
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

private:
	XWF_Call_f fnCall_;
	IGUI_t hCGUI_;
};

#endif
