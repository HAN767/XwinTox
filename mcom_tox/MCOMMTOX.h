#ifndef __GUIFLTK__H
#define __GUIFLTK__H

#include <string>

#include <threads.h>
#include <tox/tox.h>

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

private:
	static int toxLoop_(void*);
	void defaultConfig_();
	int loadToxData_();
	std::string getSaveFilePath_();

	Dictionary_t *dictConfig_;
	std::string strSavefile_, strConffile_;
	MCT_Data_t datSave_;
	Tox *tox_; 
	mtx_t mtxTox;
	thrd_t thrdTox_;
};


#endif
