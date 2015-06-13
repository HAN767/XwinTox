#ifndef CONTACTS_H_
#define CONTACTS_H_
#include <vector>
using namespace std;

extern "C"
{
#include "xwintox.h"
}

typedef struct ContactList_s
{
	int num_selected;
	size_t num_online;

	vector <Contact_t*> contacts;
} ContactList_t;

void ContactListGUIUpdate();

extern ContactList_t *contactlist;

#endif
