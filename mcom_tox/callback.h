#ifndef ___CALLBACK_H___
#define ___CALLBACK_H___

#include "AOM/IMComm.h"
#include "MCOMMTOX.h"

#define CBFUNC(NAME, ...) void cb_##NAME (Tox *tox, ##__VA_ARGS__, void *custom)

void register_callbacks(XWF_Object_Handle_t *hobjSelf);

CBFUNC(self_connection_status, TOX_CONNECTION connection_status);

CBFUNC(friend_connection_status, uint32_t friend_number,
       TOX_CONNECTION connection_status);
CBFUNC(friend_status, uint32_t friend_number, TOX_USER_STATUS status);
CBFUNC(friend_name, uint32_t friend_number, const uint8_t *name, size_t length);
CBFUNC(friend_message, uint32_t friend_number, TOX_MESSAGE_TYPE type,
       const uint8_t *message, size_t length);
CBFUNC(friend_request, const uint8_t *public_key, const uint8_t *message,
       size_t length);

/* private */
#define CBPREP XWF_Object_Handle_t *hobjSelf =custom; PREP

#endif
