#ifndef XWINTOX_H_
#define XWINTOX_H_

#ifdef __cplusplus
extern "C"
{
#endif

#include "misc.h"
#include "dictionary.h"
#include "list.h"

#include "signal.h"

typedef struct Xwin_s
{
	thrd_t Thrd;
	List_t *ICQueue;
	List_t *Events;
	mtx_t EventsMtx;
} Xwin_t;

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

#ifdef __cplusplus
extern Postbox_t *postbox;

extern XwinTox_instance_t *APP;
void CommWork();
}
#endif

#endif
