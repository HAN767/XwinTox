#ifndef _CONFIG_H
#define _CONFIG_H

#include "dictionary.h"

const char* get_home_folder();
char* get_config_folder();
char* get_config_filename();
char* get_save_filename();

int create_config_folder();
void default_config(Dictionary_t* conf);

#endif
