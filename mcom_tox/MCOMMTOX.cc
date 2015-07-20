#include <cstdio>
#include <cstring>
#include <sys/stat.h>
#include <threads.h>
#include <unistd.h>

#include "misc.h"
#include "hexstring.h"

#include "xwintox.h"
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

	registerCallbacks_();

	xwfSubscribe(clSaveData);

	xwfSubscribe(frSendMsg);
	xwfSubscribe(frDelete);
	xwfSubscribe(frAcceptRequest);
	xwfSubscribe(frSendRequest);

	thrd_create(&thrdTox_, (thrd_start_t)toxLoop_, this);

	sendFriends_();

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

void MCOMMTOX::saveToxData_()
{
	unsigned int wLen =tox_get_savedata_size(tox_);
	unsigned char *bData =new unsigned char[wLen + 1];
	FILE *hfSave;

	mtx_lock(&mtxTox_);
	tox_get_savedata(tox_, bData);
	mtx_unlock(&mtxTox_);

	mkdir(pszxwfCall("APP/GetDataFolder", 0), S_IRWXU);

	unlink(strSavefile_.c_str());
	hfSave =fopen(strSavefile_.c_str(), "wb");

	if(hfSave == NULL)
	{
		dbg("Failed to open savefile %s\n", strSavefile_.c_str());
		return;
	}

	if(fwrite(bData, wLen, 1, hfSave) != 1)
	{
		dbg("Failed to save data to savefile %s\n", strSavefile_.c_str());
		fclose(hfSave);
		return;
	}

	dbg("Saved Tox data of length %d to %s\n", wLen, strSavefile_.c_str());

	fclose(hfSave);
}

void MCOMMTOX::sendFriends_()
{
	unsigned int wCount =(tox_self_get_friend_list_size(tox_));
	if (wCount < 1) return;
	unsigned int *wNums =new unsigned int[wCount];
	List_t *lstFriends =List_new();
	PBMessage_t *msgContacts =PB_New_Message();

	tox_self_get_friend_list(tox_, wNums);

	for(int i =0; i < wCount; i++)
	{
		unsigned int wSize, wNum =wNums[i];
		char *pszText;
		unsigned char *pbPubkey;
		XWContact_t *ctNew =(XWContact_t *)calloc(1, sizeof(XWContact_t));

		ctNew->wNum =wNum;

		wSize =tox_friend_get_name_size(tox_, wNum, 0) + 1;
		pszText =new char [wSize];
		tox_friend_get_name(tox_, wNum, (uint8_t*)pszText, 0);
		pszText[wSize - 1] ='\0';
		ctNew->pszName =pszText;

		wSize =tox_friend_get_status_message_size(tox_, wNum, 0) + 1;
		pszText =new char[wSize];
		tox_friend_get_status_message(tox_, wNum, (uint8_t*)pszText, 0);
		pszText[wSize - 1] ='\0';
		ctNew->pszStatus =pszText;

		pbPubkey =new unsigned char[TOX_PUBLIC_KEY_SIZE];
		tox_friend_get_public_key(tox_, wNum, pbPubkey, 0);
		ctNew->pszID =bin_to_hex_string(pbPubkey, TOX_PUBLIC_KEY_SIZE);

		dbg("Contact: Pubkey %s, name %s, status %s\n", ctNew->pszID, ctNew->pszName, ctNew->pszStatus);
		List_add(lstFriends, ctNew);
	}
	msgContacts->V =lstFriends;
	xwfDispatch(clContacts, msgContacts);
}

void MCOMMTOX::acceptFriendRequest_(const char *pszAddress)
{
	TOX_ERR_FRIEND_ADD ferr;
	uint8_t *pk =hex_string_to_bin(pszAddress);
	unsigned int dwFNum;

	mtx_lock(&mtxTox_);
	dwFNum =tox_friend_add_norequest(tox_, pk, &ferr);
	mtx_unlock(&mtxTox_);

	dbg("FriendNoReqError: %d. Address: %s\n", ferr, pszAddress);
	if(ferr == TOX_ERR_FRIEND_ADD_ALREADY_SENT) dbg("already\n")
	else if(ferr == TOX_ERR_FRIEND_ADD_BAD_CHECKSUM) dbg("bad checksum\n")
	else
	{
		PBMessage_t *msgfrRequestServiced =PB_New_Message();
		msgfrRequestServiced->I1 =dwFNum;
		msgfrRequestServiced->S1 =strdup(pszAddress);

		xwfDispatch(frRequestServiced, msgfrRequestServiced);
	}
}


void MCOMMTOX::addFriend_(const char *addr, const char *msg)
{
	TOX_ERR_FRIEND_ADD errFriend;
	unsigned int dwNewNum;
	uint8_t* bID =hex_string_to_bin(addr);

	mtx_lock(&mtxTox_);
	dwNewNum =tox_friend_add(tox_, bID, (uint8_t *) msg,
	                      strlen(msg), &errFriend);
	mtx_unlock(&mtxTox_);

	dbg("ID %s, Msg %s, Error %d\n", addr, msg, errFriend);

	if(errFriend == TOX_ERR_FRIEND_ADD_OK)
	{
		dbg("Added friend %d\n", dwNewNum);
		PBMessage_t *msgNewFriend =PB_New_Message();

		msgNewFriend->I1 =dwNewNum;
		msgNewFriend->S1 =strdup(addr);

		xwfDispatch(frNew, msgNewFriend);
	}
	else
	{
		dbg("Add friend failed\n");
	}
}
