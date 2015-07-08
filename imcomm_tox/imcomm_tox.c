#include "Module/Module.h"
#include "AOM/IMComm.h"
#include "misc.h"

int XWF_exit()
{
	return 0;
}

int XWF_init(XWF_Module_t *pmodSelf, const XWF_Services_t *psrvServices)
{
	pmodSelf->pszName ="IMComm for Tox IM";
	pmodSelf->fnExit =XWF_exit;
	return 0;
}
