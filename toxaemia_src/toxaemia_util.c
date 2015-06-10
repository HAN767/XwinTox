#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "toxaemia_util.h"

uint8_t* hex_string_to_bin(const char *hex_string) 
{
	size_t i, len = strlen(hex_string) / 2;
	uint8_t *ret = calloc(len, 1);

	for(i = 0; i < len; i++) 
	{
		sscanf(hex_string + i * 2, "%2hhx", &ret[i]);
	}

	return ret;
}

