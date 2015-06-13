#ifndef CONTACTS_H_
#define CONTACTS_H_
#include <vector>
using namespace std;

typedef struct Contact_s
{
	char *name, *statusm, *pubkey;
	int num;
	int status; /* 0 = offline, 1 = online */
} Contact_t __attribute__((packed));

typedef struct ContactList_s
{
	int num_selected;
	size_t num_online;

	vector <Contact_t*> contacts;
} ContactList_t;

void ContactListGUIUpdate();

extern ContactList_t *contactlist;

#endif
