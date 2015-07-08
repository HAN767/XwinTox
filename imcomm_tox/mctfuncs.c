#include "AOM/IMComm.h"
#include "IMCommTox.h"
#include "mctfuncs.h"
#include "misc.h"

void MCT_Connect(IMComm_t *pimcSelf)
{
	struct Tox_Options *ptxoTopts =tox_options_new(0);

	LOCK(pimcSelf)

	if(PRIVATE(pimcSelf)->datToxSave.wLength)
	{
		ptxoTopts->savedata_type =TOX_SAVEDATA_TYPE_TOX_SAVE;
		ptxoTopts->savedata_data =(const uint8_t*)
		                          PRIVATE(pimcSelf)->datToxSave.pbData;
		ptxoTopts->savedata_length =PRIVATE(pimcSelf)->datToxSave.wLength;
		dbg("Savedata length: %d\n", ptxoTopts->savedata_length);
	}

	UNLOCK(pimcSelf)
}
