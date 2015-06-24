#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "toxaemia_util.h"

uint8_t* hex_string_to_bin(const char *hex_string)
{
	size_t i, len =strlen(hex_string) / 2;
	uint8_t *ret =calloc(len, 1);

	for(i =0; i < len; i++)
	{
		sscanf(hex_string + i * 2, "%2hhx", &ret[i]);
	}

	return ret;
}

char* bin_to_hex_string(const uint8_t *bin, size_t len)
{
	char *result =calloc((2 * len) + 1, sizeof(char));

	for(int i =0, b =0; i < (len); i++)
	{
		b += sprintf(result+b, "%02X", bin[i]);
	}

	result[(2 * len) +1] ='\0';
	return result;
}
