#ifndef __GUIFLTK__H
#define __GUIFLTK__H

#include <string>

#include <threads.h>
#include <tox/tox.h>

#include "misc.h"
#include "dictionary.h"

#include "Module/Module.h"
#include "AOM/IXWClass.h"

typedef struct MCT_Data_s
{
	char *pbData;
	unsigned int wLength;
} MCT_Data_t;

class MCOMMTOX : public XWClassT<MCOMMTOX>
{
public:
	MCOMMTOX(XWF_ObjectParams_t *pobjParams);
	int start();
	void recvSignal(unsigned int, PBMessage_t*);

private:
	static int toxLoop_(void*);
	void defaultConfig_();
	int loadToxData_();
	void saveToxData_();
	std::string getSaveFilePath_();
	void registerCallbacks_();
	void sendFriends_();

	void acceptFriendRequest_(const char *pszAddress);
	void addFriend_(const char *addr, const char *msg);

	void controlFileTransfer(unsigned int dwFID, unsigned int dwTID,
                                   unsigned int eCtrl);


	/* the Tox callbacks */
	void cb_self_connection_status(TOX_CONNECTION connection_status);

	void cb_friend_connection_status(uint32_t friend_number,
	                                 TOX_CONNECTION connection_status);
	void cb_friend_status(uint32_t friend_number, TOX_USER_STATUS status);
	void cb_friend_name(uint32_t friend_number, const uint8_t *name,
	                    size_t length);
	void cb_friend_message(uint32_t friend_number, TOX_MESSAGE_TYPE type,
	                       const uint8_t *message, size_t length);
	void cb_friend_request(const uint8_t *public_key, const uint8_t *message,
	                       size_t length);

	void cb_file_recv(uint32_t friend_number, uint32_t file_number,
	                  uint32_t kind, uint64_t file_size,
	                  const uint8_t *filename, size_t filename_length);
	void cb_file_recv_control(uint32_t friend_number,uint32_t file_number,
	                          TOX_FILE_CONTROL c);
	void cb_file_recv_chunk(uint32_t friendnumber, uint32_t filenumber,
	                        uint64_t position, const uint8_t *data,
	                        size_t length);

	/* Miscellaneous */
	TOX_FILE_CONTROL getToxFC(unsigned int eCtrl);

	Dictionary_t *dictConfig_;
	std::string strSavefile_, strConffile_;
	MCT_Data_t datSave_;
	Tox *tox_;
	mtx_t mtxTox_;
	thrd_t thrdTox_;
};

template<typename Func, Func func> struct thunk;

template<typename Class, typename R, typename ...Args, R(Class::*func)(Args...)>
struct thunk<R(Class::*)(Args...), func>
{
    static R call(Tox *tox, Args ...args, void *userData)
{
	return ((*static_cast<Class *>(userData)).*func)(args...);
}
};


/*int, PBMessage_t*, void *userData*/

#endif
