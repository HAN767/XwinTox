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

typedef enum PB_Mtypes_e
{
	/* Client related */
	clCon,

	/* Friends related */
	frCon,
	frName,
	frStatusMsg,
	frRequest,
	frAdded,

	/* Chatrooms related */
	crNew,
	crTitle,
	crNames,
	crMessage,
	crInvite,

	/* File transfers related */
	ftRequest,
	ftControl,
	ftData,
} PB_Mtypes_e;


#endif
