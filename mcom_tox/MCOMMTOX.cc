#include <cstdio>
#include <sys/stat.h>
#include <threads.h>
#include <unistd.h>

#include "misc.h"
#include "hexstring.h"

#include "AOM/IXWClass.h"
#include "Module/Module.h"
#include "MCOMMTOX.h"


MCOMMTOX::MCOMMTOX(XWF_ObjectParams_t *prmObj)
{
	dictConfig_ =Dictionary_new();
	mtx_init(&mtxTox_, mtx_plain);
	dbg("New MCOMMTOX created\n");
}

int MCOMMTOX::start()
{
	struct Tox_Options *ptxoTopts =tox_options_new(0);
	TOX_ERR_NEW errNew;
	TOX_ERR_BOOTSTRAP errBootstrap;
	uint8_t mypubkey[TOX_ADDRESS_SIZE];

	std::string strName =std::string(pszxwfCall("APP/GetName", 0));
	std::string strStatus =std::string(pszxwfCall("APP/GetName", 0));
	strSavefile_ =std::string(pszxwfCall("APP/GetDataFilename", "toxsave"));
	strConffile_ =std::string(pszxwfCall("APP/GetConfigFilename", "toxconf"));

	Dictionary_load_from_file(dictConfig_, strConffile_.c_str(), 1);
	defaultConfig_();

	ptxoTopts->ipv6_enabled =false;

	if(loadToxData_() == 0)
	{
		ptxoTopts->savedata_type =TOX_SAVEDATA_TYPE_TOX_SAVE;
		ptxoTopts->savedata_data =(const uint8_t*) datSave_.pbData;
		ptxoTopts->savedata_length =datSave_.wLength;
		dbg("Savedata length: %d\n", ptxoTopts->savedata_length);
	}

	tox_ =tox_new(ptxoTopts, &errNew);

	if(errNew != TOX_ERR_NEW_OK)
	{
		const char *pszErrcode;

		if(errNew == TOX_ERR_NEW_LOAD_BAD_FORMAT) pszErrcode =
			    "Bad save format";
		else pszErrcode ="Unknown";

		dbg("Failed to create new Tox: %s\n", pszErrcode);
		return -1;
	}

	tox_self_set_name(tox_, (uint8_t *)strName.c_str(), strName.size(), NULL);
	tox_self_set_status_message(tox_, (uint8_t *)strStatus.c_str(),
	                            strStatus.size(), NULL);

	tox_self_set_status(tox_, TOX_USER_STATUS_NONE);

	tox_bootstrap(tox_, Dictionary_get(dictConfig_, "Tox.BootstrapIP"),
	              strtol(Dictionary_get(dictConfig_, "Tox.BootstrapPort"), 0, 10),
	              hex_string_to_bin(Dictionary_get(dictConfig_, "Tox.BootstrapKey")),
	              &errBootstrap);

	if(errBootstrap != TOX_ERR_BOOTSTRAP_OK)
	{
		dbg("Failed to bootstrap\n");
	}

	tox_self_get_address(tox_, mypubkey);
	dbg("Tox ID: %s\n", bin_to_hex_string(mypubkey, TOX_ADDRESS_SIZE));

	registerCallbacks();
	xwfSubscribe(2, &MCOMMTOX::recvSignal);

	thrd_create(&thrdTox_, toxLoop_, this);

	return 0;
}

int MCOMMTOX::toxLoop_(void *userData)
{
	MCOMMTOX *self =static_cast<MCOMMTOX *>(userData);

	while (1)
	{
		mtx_lock(&self->mtxTox_);
		tox_iterate(self->tox_);
		mtx_unlock(&self->mtxTox_);
		usleep(tox_iteration_interval(self->tox_) * 1000);
	}
}

void MCOMMTOX::defaultConfig_()
{
#define Add(x,y) Dictionary_set_aux(dictConfig_, x, y)
	Add("Tox.BootstrapIP", "23.226.230.47");
	Add("Tox.BootstrapPort", "33445");
	Add("Tox.BootstrapKey",
	    "A09162D68618E742FFBCA1C2C70385E6679604B2D80EA6E84AD0996A1AC8A074");
}

int MCOMMTOX::loadToxData_()
{
	FILE *hfSave;
	struct stat st;
	off_t iLength;

	datSave_.wLength =0;

	hfSave =fopen(strSavefile_.c_str(), "rb");

	if(hfSave == NULL)
	{
		dbg("Failed to open savefile %s\n", strSavefile_.c_str());
		return 1;
	}

	stat(strSavefile_.c_str(), &st);
	iLength =st.st_size;

	if(iLength == 0)
	{
		fclose(hfSave);
		dbg("Save file unusual: length is zero\n");
		return 1;
	}

	datSave_.pbData =new char[iLength];
	datSave_.wLength =iLength;
	fread(datSave_.pbData, iLength, 1, hfSave);
	return 0;
}
