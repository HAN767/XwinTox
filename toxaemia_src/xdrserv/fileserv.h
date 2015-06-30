#ifndef __FILESERV__H__
#define __FILESERV__H__

#include "xdrserv.h"

Event_t *Ev_copy(Event_t *ev);
void Ev_pack(Event_t *ev);
void Ev_free(Event_t *ev);

extern Xdrops_t Fileops;

#endif
