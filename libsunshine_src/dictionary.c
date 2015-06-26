/*
 * LibSunshine
 *
 * Implementation of a dictionary.
 */

#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

#include "misc.h"
#include "list.h"
#include "dictionary.h"
#include "ini/ini.h"

#define _Lock_Dictionary mtx_lock(dict->Lock);
#define _Unlock_Dictionary mtx_unlock(dict->Lock);

/* internal functions */

static unsigned long
jenkins_hash (const char *key) /* Jenkins One-at-a-Time Hash function */
{
	unsigned long int hash;
	size_t len =strlen (key);

	for (int i =hash =0; i < len; ++i)
	{
		hash +=key[i];
		hash += (hash << 10);
		hash ^= (hash >> 6);
	}

	hash += (hash << 3);
	hash ^= (hash >> 11);
	hash += (hash << 15);
	return hash;
}

static Dictionary_t*
resize (Dictionary_t *dict)
{
	Dictionary_t *newdict;
	Dictionary_t tmp;
	List_t_ *e;
	Dictionary_entry_t *entry;
	mtx_t *lock =dict->Lock;

	newdict = Dictionary_new (dict->size * 2);

	for (int i = 0; i < dict->size; i++)
	{
		for (e = dict->entries[i]; e != 0; e = e->Link)
		{
			entry =e->data;
			Dictionary_set (newdict, entry->key, entry->value);
		}
	}

	tmp =*dict;
	*dict =*newdict;
	*newdict =tmp;

	Dictionary_delete (newdict);
	dict->Lock =lock;
	return dict;
}

/* primary functions */

Dictionary_t*
Dictionary_new (int size)
{
	Dictionary_t *newdict =calloc (1, sizeof (Dictionary_t));

	newdict->size =size;
	newdict->entries =calloc (1, sizeof (List_t *) * newdict->size);

	newdict->Lock =calloc(1, sizeof (mtx_t));
	mtx_init(newdict->Lock, mtx_plain);

	return newdict;
}

void
Dictionary_delete (Dictionary_t *dict)
{
	DICTIONARY_ITERATE_OPEN (dict)
	free (entry->key);
	free (entry->value);
	free (entry);
	free (e);
	DICTIONARY_ITERATE_CLOSE (dict)


	free (dict->entries);

	free (dict);
}

void
Dictionary_set (Dictionary_t *dict, const char *key, const char *value)
{
	Dictionary_entry_t *new, *e;
	List_t_ *lentry, *l;
	unsigned long hash;

	_Lock_Dictionary

	new =calloc (1, sizeof (Dictionary_entry_t));
	lentry =calloc (1, sizeof (List_t));

	new->key =strdup (key);
	new->value =strdup (value);

	hash = jenkins_hash (key) % dict->size;

	if ( (dict->count + 2) >= dict->size)
	{
		dict =resize (dict);
		assert (dict != 0);
	}

	for (l = dict->entries[hash]; l != 0; l = l->Link)
	{
		e =l->data;

		if (!strcmp (e->key, key))
		{
			free (e->value);
			e->value = new->value;
			free (new->key); free (new); free (lentry);
			_Unlock_Dictionary
			return; /* reassigned value of an entry */
		}
	}
	/* hash collision */
	{
		lentry->Link =dict->entries[hash];
		lentry->data =new;
		dict->entries[hash] =lentry;
		dict->count++;
		_Unlock_Dictionary
	}

}

const char*
Dictionary_get (Dictionary_t *dict, const char *key)
{
	List_t_ *l;
	Dictionary_entry_t *e;

	_Lock_Dictionary

	for (l = dict->entries[jenkins_hash (key) % dict->size]; l != 0; l = l->Link)
	{
		e =l->data;


		if (!strcmp (e->key, key))
		{
			_Unlock_Dictionary
			return e->value;
		}
	}

	_Unlock_Dictionary
	return 0;
}

void
Dictionary_unset (Dictionary_t *dict, const char *key)
{
	List_t_ **p;
	List_t_ *e;
	Dictionary_entry_t *entry;

	_Lock_Dictionary

	for (p =& (dict->entries[jenkins_hash (key) % dict->size]);
	        *p != 0;
	        p = & ( (*p)->Link))
	{

		entry = (*p)->data;

		if (!strcmp (entry->key, key))
		{
			e =*p;
			*p =e->Link;

			free (entry->key);
			free (entry->value);
			free (entry);
			free (e);
			_Unlock_Dictionary

			return;
		}
	}
	_Unlock_Dictionary
}

/* auxillary functions */

/* Dictionary_set_aux
 * sets value only if unset
 * returns value in either case
 */
const char*
Dictionary_set_aux (Dictionary_t *dict, const char *key, const char *value)
{
	const char* val;
	
	if (!(val =Dictionary_get(dict, key)))
	{
		Dictionary_set (dict, key, value); 
		return value;
	}
	else return val;
}

/* Dictionary_set_if_exists
 * sets value if key already exists
 * returns 0 for success and 1 if key doesn't exist
 */
int
Dictionary_set_if_exists (Dictionary_t *dict, const char *key, const char *value)
{
	if (Dictionary_get(dict, key))
	{
		Dictionary_set (dict, key, value); 
		return 0;
	}
	else return 1;
}

/* Utility function for Dictionary_load_from_file()
 */
int
parse_config_line (void* user, const char* section, const char* name,
                   const char* value)
{
	char key[255];
	snprintf (key, 255, "%s.%s", section, name);
	Dictionary_set ((Dictionary_t*) user, key, value);
	return 1;
}

/* Dictionary_load_from_file
 * loads a dictionary with a file exported by Dictionary_export_to_file()
 * returns 0 for success, -1 for file created, 1 for unable to open file,
 * 2 for unable to open file and could not create it, and 3 for ini error 
 * reading file.
 */
int
Dictionary_load_from_file (Dictionary_t *dict, const char *filename, 
int createifnotexist)
{
	int inierror;
	FILE* dictFILE =fopen (filename, "r");

	if (dictFILE == NULL && createifnotexist)
	{
		printf ("Failed to open config file\nAttempting to create one\n");
		dictFILE =fopen (filename, "w+");

		if (dictFILE == NULL)
		{
			printf ("Failed to create config file!\n");
			return -2;
		}

		fclose (dictFILE);
		return -1;
	}
	else if (dictFILE == NULL)
	{ printf("Failed to open config file\n"); return 0; }

	inierror =ini_parse_file (dictFILE, parse_config_line, dict);

	if (inierror > 0) dbg ("Error starting line %d of config file\n", inierror);

	if (inierror < 0) { dbg ("Error reading file %s\n", tmpfile); return 3; }

	return 0;
}

int
Dictionary_write_to_file (Dictionary_t *dict, const char *filename)
{
	FILE* dictFILE;
	List_t *secbuffer =List_new();

	DICTIONARY_ITERATE_OPEN (dict)
		char *tofree, *section, *name, *secbuf, secbegin[255], secline[8192];
		secbuf =0;

		tofree = name = strdup (entry->key);
		section =strsep (&name, ".");

		sprintf (secbegin, "[%s]\n", section); /* fine */

		LIST_ITERATE_OPEN(secbuffer)
			if (!strncmp (e->data, secbegin, strlen (secbegin))) secbuf =e->data;
		LIST_ITERATE_CLOSE(secbuffer)

		if (!secbuf)
		{
			secbuf =calloc (8192, sizeof (char));
			sprintf (secbuf, "%s", secbegin);
			List_add (secbuffer, secbuf);
		}

		sprintf (secline, "%s=%s\n", name, entry->value);
		strcat (secbuf, secline);

		free (tofree);
	DICTIONARY_ITERATE_CLOSE(dict)

	dictFILE =fopen (filename, "w");

	if (dictFILE == NULL)
	{
		dbg ("Failed to open config file (%s) for writing\n", filename);
		return 1;
	}

	LIST_ITERATE_OPEN(secbuffer)
		fprintf (dictFILE, "%s", e->data);
	LIST_ITERATE_CLOSE(secbuffer)

	fclose (dictFILE);
	return 0;
}
