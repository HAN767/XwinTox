#include "misc.h"

#include "Module/Module.h"
#include "AOM/IMComm.h"
#include "module/manager.h"

int main(int argc, char *argv[])
{
	dbg("XwinTox Frameworks 2.0_%s\n", XWVERS);
	ModuleManager_init();
	ModuleManager_loadDynamicModule("/home/david/.histfile");
	return 0;
}
