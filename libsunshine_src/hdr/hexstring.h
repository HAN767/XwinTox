#ifndef TOXAEMIA_UTIL_H
#define TOXAEMIA_UTIL_H

#ifdef __cplusplus
extern "C"
{
#endif

uint8_t* hex_string_to_bin(const char *hex_string);
char* bin_to_hex_string(const uint8_t *bin, size_t len);

#ifdef __cplusplus
}
#endif

#endif

