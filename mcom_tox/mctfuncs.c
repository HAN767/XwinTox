#include <stdlib.h>
#include <string.h>

#include "AOM/IMComm.h"
#include "MCOMMTOX.h"
#include "mctfuncs.h"
#include "hexstring.h"
#include "misc.h"

#define PREP IMComm_t *pimcSelf =hobjSelf->hObj;
#define TOXINST PRIVATE(pimcSelf)->ptoxTox

int MCT_Connect(XWF_Object_Handle_t *hobjSelf)
{
	PREP
	TOX_ERR_NEW txnerrTnerr;
	TOX_ERR_BOOTSTRAP txberrTberr;
	struct Tox_Options *ptxoTopts =tox_options_new(0);

	LOCK(pimcSelf)

	ptxoTopts->ipv6_enabled =false;

	if(PRIVATE(pimcSelf)->datToxSave.wLength)
	{
		ptxoTopts->savedata_type =TOX_SAVEDATA_TYPE_TOX_SAVE;
		ptxoTopts->savedata_data =(const uint8_t*)
		                          PRIVATE(pimcSelf)->datToxSave.pbData;
		ptxoTopts->savedata_length =PRIVATE(pimcSelf)->datToxSave.wLength;
		dbg("Savedata length: %d\n", ptxoTopts->savedata_length);
	}

	TOXINST =tox_new(ptxoTopts, &txnerrTnerr);

	if(txnerrTnerr != TOX_ERR_NEW_OK)
	{
		const char *pszErrcode;

		if(txnerrTnerr == TOX_ERR_NEW_LOAD_BAD_FORMAT) pszErrcode =
			    "Bad save format";
		else pszErrcode ="Unknown";

		dbg("Failed to create new Tox: %s\n", pszErrcode);
		return -1;
	}

	tox_self_set_name(TOXINST, (uint8_t*)pimcSelf->pszName,
	                  strlen(pimcSelf->pszName), NULL);
	tox_self_set_status_message(TOXINST, (uint8_t*)pimcSelf->pszStatus,
	                            strlen((char*)(pimcSelf->pszStatus)),
	                            NULL);

	tox_self_set_status(TOXINST, TOX_USER_STATUS_NONE);

	tox_bootstrap(TOXINST,
	              Dictionary_get(pimcSelf->dictConfig, "Tox.BootstrapIP"),
	              strtol(
	                  Dictionary_get(pimcSelf->dictConfig, "Tox.BootstrapPort"),
	                  0, 10),
	              hex_string_to_bin(
	                  Dictionary_get(pimcSelf->dictConfig, "Tox.BootstrapKey")),
	              &txberrTberr);

	if(txberrTberr != TOX_ERR_BOOTSTRAP_OK)
	{
		dbg("Failed to bootstrap\n");
	}

	UNLOCK(pimcSelf)
	return 0;
}
