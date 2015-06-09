/*
 * LibSunshine
 *
 * Function & type definitions for dictionary implementation.
 */

#ifndef Dictionary_h
#define Dictionary_h

#include "list.h"
#include "misc.h" /* threading support */

typedef struct Dictionary_entry
{
	char *key;
	char *value;
} Dictionary_entry_t;

typedef struct Dictionary
{
	int size;
	int count;
	List_t **entries; /* array of list_t*s pointing to of dictionary_entry_ts */
	_Lock_
} Dictionary_t;

#define DICTIONARY_ITERATE_OPEN(DICT) \
	List_t *e, *next; \
	Dictionary_entry_t *entry; \
	_Locked_Start(( DICT )->_Lock) \
	for (int i = 0; i < ( DICT )->size; i++) \
	{ \
		for (e = ( DICT )->entries[i]; e != 0; e = next) \
		{ \
			next =e->Link; \
			entry =e->data; \
			 

#define DICTIONARY_ITERATE_CLOSE(DICT) _Locked_End(( DICT )->_Lock) } }

Dictionary_t *Dictionary_new();
void Dictionary_delete (Dictionary_t *dict);
void Dictionary_set (Dictionary_t *dict, const char *key, const char *value);
const char *Dictionary_get (Dictionary_t *dict, const char *key);
void Dictionary_unset (Dictionary_t *dict, const char *key);

const char *Dictionary_set_aux (Dictionary_t *dict, const char *key,
                                const char *value);
int Dictionary_set_if_exists (Dictionary_t *dict, const char *key,
                                const char *value);

int Dictionary_load_from_file (Dictionary_t *dict, const char *filename, 
								int createifnotexist);
int Dictionary_write_to_file (Dictionary_t *dict, const char *filename);

#endif
