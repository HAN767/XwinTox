#include <errno.h> // errno
#include <stdio.h> // snprintf()
#include <stdlib.h> // getenv()
#include <string.h> // strerror()
#include <sys/types.h> 
#include <sys/stat.h> // stat, struct stat
#include <unistd.h> // getuid()
#include <pwd.h> // getpwuid()

#include "misc.h"
#include "dictionary.h"

#include "etc.h"

const char* get_home_folder()
{
	const char *homefolder;

	if ((homefolder =getenv("HOME")) == NULL) 
	{
		homefolder =getpwuid(getuid())->pw_dir;
	}

	return homefolder;
}

char* get_config_folder()
{
	char *configfolder;

	configfolder =calloc(1, 255);
	snprintf(configfolder, 255, "%s/.XwinTox", get_home_folder());

	return configfolder;
}

char* get_config_filename()
{
	char *configfilename;

	configfilename =calloc(1, 255);
	snprintf(configfilename, 255, "%s/XwinTox.ini", get_config_folder());

	return configfilename;
}

int create_config_folder()
{
	int error;
	char *cfgfolder =get_config_folder();

	if ((error =mkdir(cfgfolder, 0700)) == -1)
	{
		struct stat s;
		if (errno == EEXIST && ! stat(cfgfolder, &s) &&
			s.st_mode & S_IFDIR) { free (cfgfolder); return 0; }
		else 
		{ 
			dbg("Failed to create config folder: %s", strerror(errno));
			free(cfgfolder); 
			return 1; 
		}
	}

	free(cfgfolder);
	return 0;
}

void default_config(Dictionary_t *conf)
{
	#define Add(x,y) Dictionary_set(conf, x, y)
	Add("XwinTox.Version", XWTOXVERSION);
	Add("Tox.Name", "XwinTox User");
	Add("Tox.Status", "Toxing on XwinTox");
	Add("Tox.BootstrapIP", "23.226.230.47");
	Add("Tox.BootstrapPort", "33445");
	Add("Tox.BootstrapKey", 
		"A09162D68618E742FFBCA1C2C70385E6679604B2D80EA6E84AD0996A1AC8A074");
}		

