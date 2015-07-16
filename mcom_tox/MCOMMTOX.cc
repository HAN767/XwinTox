#include <cstdio>
#include <sys/stat.h>

#include "misc.h"

#include "AOM/IXWClass.h"
#include "Module/Module.h"
#include "MCOMMTOX.h"


MCOMMTOX::MCOMMTOX(XWF_ObjectParams_t *prmObj)
{
	dictConfig_ =Dictionary_new();
	dbg("New MCOMMTOX created\n");
}

int MCOMMTOX::start()
{
	struct Tox_Options *ptxoTopts =tox_options_new(0);

	strSavefile_ =std::string(pszxwfCall("APP/GetDataFilename", "toxsave"));
	strConffile_ =std::string(pszxwfCall("APP/GetConfigFilename", "toxconf"));

	Dictionary_load_from_file(dictConfig_, strConffile_.c_str(), 1);
	defaultConfig_();

	if (loadToxData_() == 0)
	{
		ptxoTopts->savedata_type =TOX_SAVEDATA_TYPE_TOX_SAVE;
		ptxoTopts->savedata_data =(const uint8_t*) datSave_.pbData;
		ptxoTopts->savedata_length =datSave_.wLength;
		dbg("Savedata length: %d\n", ptxoTopts->savedata_length);
	}

	thrd_create(&thrdTox_, toxLoop_, 0);
	return 0;
}

int MCOMMTOX::toxLoop_(void*)
{

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
