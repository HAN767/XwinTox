#include <stdio.h>
#include <string.h>

#include "misc.h"

#include "Module/Module.h"
#include "AOM/IMComm.h"
#include "module/manager.h"
#include "xwt.h"

XWT_t App;

static void *AppCall(const XWF_Object_Handle_t *pobjhSource,
                     const char * pszService,
                     const void *pvParams)
{
	if(strcmp(pszService, "GetConfigFilename") == 0)
	{
		static char szPath[255];
		snprintf(szPath, 255, "%s/.XwinTox/%s.ini", get_home_folder(), pvParams);
		return szPath;
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
	IMComm_t *pimcM;

	dbg("XwinTox Frameworks 2.0_%s\n", XWVERS);

	App.dictConfig =Dictionary_new();
	snprintf(szConfigFilename, 255, "%s/%s", get_home_folder(),
	         ".XwinTox/XwinTox.ini");
	Dictionary_load_from_file(App.dictConfig, szConfigFilename, 1);
	default_config(App.dictConfig);

	ModuleManager_init(AppCall);
	ModuleManager_loadDynamicModule
	("/ws/tox/XwinTox/imcomm_tox/libimcomm_tox.so");
	ModuleManager_loadDynamicModule
	("/ws/tox/XwinTox/gui_fltk/libgui_fltk.so");

	App.pimcIM =ModuleManager_createObject("MESSENGER");
	App.pguiGUI =ModuleManager_createObject("GUI");
	pimcM =App.pimcIM->pobjObject;
	pimcM->pszName =strdup(Dictionary_get(App.dictConfig, "XwinTox.Name"));
	pimcM->pszStatus =strdup(Dictionary_get(App.dictConfig, "XwinTox.Status"));

	pimcM->fnConnect(pimcM);
	GUIOBJ(App.pguiGUI)->fnStart(App.pguiGUI);

	ModuleManager_destroyObject(App.pimcIM);
	Dictionary_write_to_file(App.dictConfig, szConfigFilename);
	return 0;
}
