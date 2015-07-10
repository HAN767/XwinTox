#ifndef __XWINTOX_H__
#define __XWINTOX_H__

#include "AOM/IMComm.h"
#include "AOM/IGUI.h"

typedef struct XWT_s
{
	Dictionary_t *dictConfig;
	XWF_Object_Handle_t *pimcIM;
	XWF_Object_Handle_t *pguiGUI;
} XWT_t;

#endif
