#ifndef CONTACTS_H_
#define CONTACTS_H_
#include <vector>
using namespace std;

#include <tox/tox.h>

typedef struct Contact_s
{
	char name[256];
	char statusm[TOX_MAX_STATUS_MESSAGE_LENGTH + 1];
	char pub_key[TOX_PUBLIC_KEY_SIZE];

	int num;
	TOX_CONNECTION connection_status;

	int status;
} Contact_t;

typedef struct ContactList_s
{
	int num_selected;
	size_t num_online;

	vector <Contact_t*> contacts;
} ContactList_t;

void ContactListGUIUpdate();

extern ContactList_t *contactlist;

#endif
