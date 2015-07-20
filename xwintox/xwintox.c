#include <stdio.h>
#include <string.h>
#include <unistd.h>

#include "misc.h"

#include "Module/Module.h"
#include "AOM/IXWClass.h"
#include "module/manager.h"
#include "xwt.h"

XWT_t App;

static void *AppCall(const XWF_Object_Handle_t *pobjhSource,
                     const char * pszService,
                     const void *pvParams)
{
	if(strcmp(pszService, "GetName") == 0)
	{
		return (void*)Dictionary_get(App.dictConfig, "XwinTox.Name");
	}
	else if(strcmp(pszService, "GetStatus") == 0)
	{
		return (void*)Dictionary_get(App.dictConfig, "XwinTox.Status");
	}
	else if(strcmp(pszService, "GetConfigFilename") == 0)
	{
		static char szPath[255];
		snprintf(szPath, 255, "%s/.XwinTox/%s.ini", get_home_folder(), pvParams);
		return szPath;
	}
	else if(strcmp(pszService, "GetDataFolder") == 0)
	{
		static char szDir[255];
		snprintf(szDir, 255, "%s/.XwinTox", get_home_folder());
		return szDir;
	}
	else if(strcmp(pszService, "GetDataFilename") == 0)
	{
		static char szPath[255];
		snprintf(szPath, 255, "%s/.XwinTox/%s.dat", get_home_folder(), pvParams);
		return szPath;
	}
	else if(strcmp(pszService, "GetDictionaryEntry") == 0)
	{
		return (void*)Dictionary_get(App.dictConfig, pvParams);
	}
	else
	{
		dbg("Unhandled APP call: <%s>\n");
	}

	return 0;
}

static void default_config(Dictionary_t *conf)
{
#define Add(x,y) Dictionary_set_aux(conf, x, y)
	Add("XwinTox.Version", XWVERS);
	Add("XwinTox.Name", "XwinTox User");
	Add("XwinTox.Status", "Toxing on XwinTox");
}

int main(int argc, char *argv[])
{
	char szConfigFilename[255];

	dbg("XwinTox Frameworks 2.0_%s\n", XWVERS);

	App.dictConfig =Dictionary_new();
	snprintf(szConfigFilename, 255, "%s/%s", get_home_folder(),
	         ".XwinTox/XwinTox.ini");
	Dictionary_load_from_file(App.dictConfig, szConfigFilename, 1);
	default_config(App.dictConfig);

	ModuleManager_init(AppCall);
	ModuleManager_loadDynamicModule
	("/ws/tox/XwinTox/out/freebsd.amd64/release/stage/lib/mcom_tox.so");
	ModuleManager_loadDynamicModule
	("/ws/tox/XwinTox/out/freebsd.amd64/release/stage/lib/GUI_FLTK.so");

	App.objMSGR =ModuleManager_createObject("MESSENGER");
	App.objGUI =ModuleManager_createObject("GUI");

	XWOBJ(App.objGUI)->fnStart(App.objGUI);
	usleep(750); /* let the GUI initialise, ready for signal reception */

	XWOBJ(App.objMSGR)->fnStart(App.objMSGR);

	while(1)
	{
		sleep(1);
	}

	//ModuleManager_destroyObject(App.pimcIM);
	Dictionary_write_to_file(App.dictConfig, szConfigFilename);
	return 0;
}
