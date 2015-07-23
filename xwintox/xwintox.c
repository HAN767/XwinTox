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
	else if(strcmp(pszService, "GetHomeFolder") == 0)
	{
		return (void*)get_home_folder();
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
	int c, index, modcount =0;
	char szConfigFilename[255], *next, *mods[argc];

	dbg("XwinTox Frameworks 2.0_%s\n", XWVERS);

	App.dictConfig =Dictionary_new();
	snprintf(szConfigFilename, 255, "%s/%s", get_home_folder(),
	         ".XwinTox/XwinTox.ini");
	Dictionary_load_from_file(App.dictConfig, szConfigFilename, 1);
	default_config(App.dictConfig);

	ModuleManager_init(AppCall);

	while((c = getopt(argc, argv, "m:-")) != -1)
	{
		switch(c)
		{
		case 'm':
			index = optind-1;

			while(index < argc)
			{
				next = strdup(argv[index]);
				index++;

				if(next[0] != '-')
				{
					mods[modcount++] = next;
				}
				else break;
			}

			optind = index - 1;
			break;
		}
	}

	for(int i =0; i < modcount; i++)
	{
		if (ModuleManager_loadDynamicModule(mods[i]) == -1) exit(1);
	}

	for(index = optind; index < argc; index++)
	{
		XWF_Object_Handle_t *objCur =ModuleManager_createObject(argv[index]);
		if(!objCur)
		{
			dbg("Error creating class %s. Did you forget to pass -- on the" \
				"command line?\n", argv[index]);
			exit(1);
		}
		XWOBJ(objCur)->fnStart(objCur);
	}

	while(1)
	{
		sleep(1);
	}

	//ModuleManager_destroyObject(App.pimcIM);
	Dictionary_write_to_file(App.dictConfig, szConfigFilename);
	return 0;
}
