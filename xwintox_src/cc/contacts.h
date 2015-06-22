#ifndef CONTACTS_H_
#define CONTACTS_H_

#include <vector>

using namespace std;

extern "C"
{
#include "xwintox.h"
}

#include "control/cntctent.h"

typedef struct ContactList_s
{
	vector <Contact_t*> contacts;
} ContactList_t;

extern ContactList_t *contactlist;
extern vector <Groupchat_t*> groupchats;

void FriendRequestSuccess(int num);
void DeleteContact(int num);

void GroupchatCreateSuccess(int num);
void GroupchatNames(int num, int numpeers, char* names, char* names_raw,
					short *names_raw_lens, int names_raw_len);
char* GroupchatGetPeerName(int gnum, int pnum);

void ContactListGUIUpdate();

class ContactsEntry;
class GMessageArea;

ContactsEntry *FindContactEntry(unsigned int num);
GMessageArea *FindContactMArea(Contact_t *contact);
GMessageArea *FindContactMArea(unsigned int id);
Contact_t *FindContact(unsigned int id);

Groupchat_t *FindGroupchat(unsigned int id);
GMessageArea *FindGroupchatMArea(Groupchat_t *contact);
GMessageArea *FindGroupchatMArea(unsigned int id);

char *GetDisplayName(Contact_t *contact, size_t LenLimit);
char *GetDisplayStatus(Contact_t *contact, size_t LenLimit);


#endif
