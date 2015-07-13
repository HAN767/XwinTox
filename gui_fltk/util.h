#ifndef ___UTIL_H___
#define ___UTIL_H___

#include "xwintox.h"
#include "control/xwin.h"
#include "control/msgarea.h"
#include "control/cntctent.h"

ContactsEntry *FindContactEntry(XwinTox *Xw, unsigned int num);
GMessageArea *FindContactMArea(XwinTox *Xw, XWContact_t *contact);
GMessageArea *FindContactMArea(XwinTox *Xw, unsigned int id);
//XWContact_t *FindContact(XwinTox *Xw, unsigned int id);


#endif

