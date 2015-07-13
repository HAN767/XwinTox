#ifndef ___CALLBACK_H___
#define ___CALLBACK_H___

#include "AOM/IMComm.h"
#include "MCOMMTOX.h"

#define CBFUNC(NAME, ...) void cb_##NAME (Tox *tox, ##__VA_ARGS__, void *custom)

void register_callbacks(XWF_Object_Handle_t *hobjSelf);
CBFUNC(self_connection_status, TOX_CONNECTION connection_status);

/* private */
#define CBPREP XWF_Object_Handle_t *hobjSelf =custom; PREP

#endif
