#ifndef __XWINTOX_H__
#define __XWINTOX_H__

#include <stdio.h>
#include "dictionary.h"
#include "Module/Module.h"

typedef struct XWT_s
{
    Dictionary_t * dictConfig;
    XWF_Object_Handle_t * objMSGR;
    XWF_Object_Handle_t * objGUI;
} XWT_t;

typedef enum XWMtypes_e
{
    /* Client related */
    clConn,     /* I1 = 0 Offline, 1 Online */
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

    frAvDownloaded,

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
     * I3 = Position. I4 = Length.
     * V = Data */
    ftRequest,
    ftControl,
    ftBytes,

    /* ftSendRequest = MCOM says to transfer a file
     * I1 = Friend number. I2 = File number.
     * I3 = file size. S1 = full path.
     * S2 = file name. */
    ftSendRequest,

    /* ftResume = Resume a file transfer (or start one)
     * I1 = Friend number. I2 = File number. */

    /* ftSend = Widget wants to transfer a file.
     * A MESSENGER object can handle this and deliver an
     * ftSendRequest to the GUI.
     * I1 = Friend number. S1 = Filename (full path).
     * I2 = File size. */
    ftResume,
    ftSend,
} XWMtypes_e;

typedef struct XWContact_s
{
    unsigned int wNum, wStatus;
    char * pszName, *pszStatus, *pszID;
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
    char * pszName, *arpszPeers[];
} XWGroupchat_t;

/* Frees an XWContact_t
 * It is left to the MCOM and GUI modules to free their own stuff if they want*/
static inline void freeContact (XWContact_t * ctToFree)
{
    if (ctToFree->pszName)
        free (ctToFree->pszName);
    if (ctToFree->pszStatus)
        free (ctToFree->pszStatus);
    if (ctToFree->pszID)
        free (ctToFree->pszID);
}

typedef struct XWFriendRequest_s
{
    char * pszAddress, *pszMessage;
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
    const char * filename;
    XWContact_t * contact;
    unsigned int num, size, donebytes;
    struct tm * time;
    FILE * file;
} XWTransfer_t;

#endif
