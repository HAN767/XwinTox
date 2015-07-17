#ifndef __XWINTOX_H__
#define __XWINTOX_H__

#include <stdio.h>
#include "dictionary.h"

//#include "AOM/IMComm.h"
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
	frRequestServiced,
	frMsg,

	frDelete,
	frMComDelete, /* issued by frDelete handler in GUI after it's cleared up */
	frSendMsg,
	frAcceptRequest,
	frSendRequest,

    /* Chatrooms related */
    crNew,
    crTitle,
    crNames,
    crMessage,
    crInvite,

    /* File transfers related */
	/* ftRequest = File delivery request
	 * I1 = Friend number. I2 = File number.
	 * I3 = File length. S1 = File name. */

	/* ftControl = File control
	 * I1 = Friend number. I2 = File number.
	 * I3 = (Enum TransferControl) Control ID */

	/* ftBytes = File data delivery
	 * I1 = Friend number. I2 = File number.
	 * I3 = Position. V = Data */
    ftRequest,
    ftControl,
    ftBytes,
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

typedef struct XWFriendRequest_s
{
	char *pszAddress, *pszMessage;
} XWFriendRequest_t;

typedef enum XWFTransferControl_e
{
	TC_Resume,
	TC_Pause,
	TC_Cancel
} XWFTransferControl_e;

typedef enum TransferState_e
{
    TR_Waiting,
    TR_Active,
    TR_Pause
} TransferState_e;

typedef enum TransferDir_e
{
	TR_Recv,
	TR_Send
} TransferDir_e;

typedef struct XWTransfer_s
{
	TransferState_e state;
	TransferDir_e dir; /* dir = 0 means receive, 1 means send */
	const char *filename;
	XWContact_t *contact;
	unsigned int num, size, donebytes;
	struct tm *time;
	FILE* file;
} XWTransfer_t;


#endif
