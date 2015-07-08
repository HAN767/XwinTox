#include "misc.h"

#include "Module/Module.h"
#include "AOM/IMComm.h"
#include "module/manager.h"

int main(int argc, char *argv[])
{
	dbg("XwinTox Frameworks 2.0_%s\n", XWVERS);
	ModuleManager_init();
	ModuleManager_loadDynamicModule
	("/ws/tox/XwinTox/imcomm_tox/libimcomm_tox.so");
	return 0;
}
