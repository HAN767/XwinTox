#include "misc.h"

#include "Module/Module.h"
#include "AOM/IMComm.h"
#include "module/manager.h"
#include "xwt.h"

XWT_t App;

void *AppCall(const char *pszService, const void *pvParams);

int main(int argc, char *argv[])
{
	dbg("XwinTox Frameworks 2.0_%s\n", XWVERS);
	ModuleManager_init(&App, AppCall);
	ModuleManager_loadDynamicModule
	("/ws/tox/XwinTox/imcomm_tox/libimcomm_tox.so");

	App.pimcIM =ModuleManager_createObject("IM");
	return 0;
}

void *AppCall(const char *pszService, const void *pvParams)
{
	dbg("APP call: %s\n", pszService)
	return 0;
}
