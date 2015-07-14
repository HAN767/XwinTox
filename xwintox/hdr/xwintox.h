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

typedef enum XWMtypes_e
{
    /* Client related */
    clConn, /* I1 = 0 Offline, 1 Online */
    clContacts, /* V = pointer to List_t of XWContact_t*s */

    clSaveData,

    /* Friends related */
	frNew,
    frStatus,
    frName,
    frStatusMsg,
    frRequest,
    frAdded,
	frMsg,

	frDelete,
	frMComDelete, /* issued by frDelete handler in GUI after it's cleared up */
	frSendMsg,

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
} XWMtypes_e;

typedef struct XWContact_s
{
	unsigned int wNum, wStatus;
	char *pszName, *pszStatus, *pszID;
} XWContact_t;

typedef enum XWContactStatus_e
{
	stOffline,
	stOnline,
	stAway,
	stBusy
} XWContactStatus_e;

typedef struct XWGroupchat_s
{
	unsigned int wNum;
	char *pszName, *arpszPeers[];
} XWGroupchat_t;

/* Frees an XWContact_t
 * It is left to the MCOM and GUI modules to free their own stuff if they want*/
static inline void freeContact(XWContact_t *ctToFree)
{
	if (ctToFree->pszName) free (ctToFree->pszName);
	if (ctToFree->pszStatus) free (ctToFree->pszStatus);
	if (ctToFree->pszID) free (ctToFree->pszID);
}


#endif
