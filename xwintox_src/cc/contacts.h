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

extern ContactList_t *contactlist;
extern vector <Groupchat_t*> groupchats;

void ContactListGUIUpdate();

class ContactsEntry;
class GMessageArea;

ContactsEntry *FindContactEntry(unsigned int num);
GMessageArea *FindContactMArea(Contact_t *contact);
GMessageArea *FindContactMArea(unsigned int id);
Contact_t *FindContact(unsigned int id);

Groupchat_t *FindGroupchat(unsigned int id);

char *GetDisplayName(Contact_t *contact, size_t LenLimit);
char *GetDisplayStatus(Contact_t *contact, size_t LenLimit);

void FriendRequestSuccess(int num);
void DeleteContact(int num);

void GroupchatCreateSuccess(int num);

#endif
