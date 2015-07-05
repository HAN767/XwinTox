#ifndef XWINTOX_H_
#define XWINTOX_H_

#ifdef __cplusplus
extern "C"
{
#endif

#include "misc.h"
#include "dictionary.h"
#include "list.h"

typedef enum PBMTypes_e
{
	PB_FRequest =1,
	PB_FReqServiced =2,
	PB_DNSResolved =4,
	PB_TRequest =8,
} PBMTypes_e;

#include "postbox.h"
#include "xdrcomm/xdrcomm.h"

typedef enum CommEventTypes_e
{
/* Misc and friend handling */
	Comm_Quit,
	Comm_SaveData,
	Comm_ChangeName,

	Comm_GetFriendList,
	Comm_SendFriendRequest,
	Comm_AddFriendNoReq,
	Comm_DeleteFriend,

/* Messaging */
	Comm_SendMessage,

/* Groupchats */
	Comm_NewGroupchat,
	Comm_LeaveGroupchat,
} CommEventTypes_e;

typedef struct Xwin_s
{
	thrd_t Thrd;
	List_t *ICQueue;
	List_t *Events;
	mtx_t EventsMtx;
} Xwin_t;

typedef struct Resolv_s
{
	thrd_t Thread;
	List_t *Calls;
	short CallsCnt;
	mtx_t CallsMtx;
	cnd_t CallsCnd;
} Resolv_t;

typedef struct Comm_s
{
	int WantQuit;
	int Work;
	mtx_t WorkMtx;
	List_t *WorkQueue;
} Comm_t;


typedef struct XwinTox_instance_s
{
	Xwin_t *Xwin;
	Comm_t *Comm;
	Resolv_t *Resolv;
	Xdrcomm_t *Evcomm, *Filecomm;

	Postbox_t *events;

	int Connected;

	Dictionary_t *Config;
	char *ConfigFilename;
} XwinTox_instance_t;

typedef struct Contact_s
{
	unsigned int num;
	char *name, *statusm, *pubkey;
	int status, connected; /* connected: 0 = offline, 1 = online */
} Contact_t;

typedef struct Groupchat_s
{
	unsigned int num, num_peers, peers_raw_len;
	char *name, *peers, *peers_raw;
	short *peers_raw_lens;
} Groupchat_t;

typedef struct FriendRequest_s
{
	char *pubkey, *message;
} FriendRequest_t;

extern XwinTox_instance_t *APP;

#ifdef __cplusplus
void CommWork();
}
#endif

#endif
