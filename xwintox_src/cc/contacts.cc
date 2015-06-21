#include <vector>

extern "C"
{
#include "misc.h"
}
#include "contacts.h"
#include "xwintox_win.h"

ContactList_t *contactlist;
vector <Groupchat_t*> groupchats;

void SendMessagePressed(Fl_Widget* B , void*);

void DeleteContact(int num)
{
	Contact_t *todel;
	char *amsg =(char*)calloc(255, sizeof(char));
	if((todel =FindContact(num)) != 0)
	{
		vector <Contact_t*> *ref=&contactlist->contacts;
		sprintf(amsg, "deletecontact %d", num);
		List_add(&APP->Comm->WorkQueue, (void*)amsg);
		free(todel->name); free(todel->statusm); free(todel->pubkey);
		free(todel);
		ref->erase(std::remove(ref->begin(), ref->end(), todel), ref->end());
		if(XwinTox->contents->currentarea == FindContactMArea(todel))
		{
			XwinTox->contents->currentarea =XwinTox->contents->addfriend;
		}
		CommWork();
	}
}

void FriendRequestSuccess(int num)
{
	if(!FindContact(num))
	{
	Contact_t *c =(Contact_t*)calloc(1, sizeof(Contact_t));
	c->name =(char*)calloc (8, sizeof(char)); strcpy(c->name, "Unknown");
	c->num =num;
 	c->statusm =strdup(""); 
	c->pubkey =strdup("");
	contactlist->contacts.push_back(c);
	ContactListGUIUpdate();
	}
}

void GroupchatCreateSuccess(int num)
{
	if(!FindGroupchat(num))
	{
	Groupchat_t *g =new Groupchat_t;
	g->num =num;
	g->name =(char*)calloc (15, sizeof(char)); 
	sprintf(g->name, "Groupchat %d", num + 1);
	g->peers =strdup("");
	groupchats.push_back(g);
	ContactListGUIUpdate();
	}
}

void GroupchatNames(int num, int numpeers, char* names, char* names_raw,
					short *names_raw_lens, int names_raw_len)
{
	Groupchat_t *g;
	if((g =FindGroupchat(num)) == 0) { dbg("No groupchat\n"); return; }
	g->peers =strdup(names);
	g->peers_raw_len =names_raw_len;
	g->num_peers =numpeers;
	g->peers_raw =(char*)calloc(names_raw_len, sizeof(char));
	memcpy(g->peers_raw, names_raw, names_raw_len);
	g->peers_raw_lens =(short*)calloc(numpeers, sizeof(short));
	memcpy(g->peers_raw_lens, names_raw_lens, sizeof(short) * numpeers);
	FindGroupchatMArea(num)->redraw();
	FindGroupchatMArea(num)->gnames->redraw();
}

char* GroupchatGetPeerName(int gnum, int pnum)
{
	static char tmpname[128] = { 0 };
	Groupchat_t *g =FindGroupchat(gnum);
	
	memcpy(tmpname, g->peers_raw + (pnum * 128), g->peers_raw_lens[pnum]);
	tmpname[g->peers_raw_lens[pnum]] = '\0';

	return (char*)(&tmpname);
}

void ContactListGUIUpdate()
{
	int selected =-1;
	int seltype =-1;

	if(XwinTox->sidebar->contacts->entries.size() > 0)
	{
		if (XwinTox->sidebar->contacts->selected >= 0)
		{ selected =XwinTox->sidebar->contacts->selected;
		  seltype =XwinTox->sidebar->contacts->seltype; }
		XwinTox->sidebar->contacts->clear_all();
	}

	int YM =0;
	for (const auto groupchat : groupchats)
	{
		printf("Contact: %s\n", groupchat->name);
		ContactsEntry *newgui =new ContactsEntry(XwinTox->sidebar->contacts->x(), 
											  XwinTox->sidebar->contacts->y() + YM, 
											  XwinTox->sidebar->contacts->scale, 
											  0, groupchat, 1);
		XwinTox->sidebar->contacts->add(newgui);
		XwinTox->sidebar->contacts->entries.push_back(newgui);
		YM += (50 * XwinTox->scale);

		if(!FindGroupchatMArea(groupchat))
		{
			GMessageArea *newarea =new GMessageArea(XwinTox->sidebar->scale, 0,
													groupchat, 1);
			newarea->hide();
			newarea->send->callback(&SendMessagePressed);

			XwinTox->add(newarea);
			XwinTox->contents->messageareas.push_back(newarea);
		}

		if(groupchat->num == selected && seltype == 1)
		{
			newgui->selected =1;
			XwinTox->contents->NewCurrentArea(FindGroupchatMArea(groupchat));
			XwinTox->sidebar->contacts->selected =selected;
			XwinTox->sidebar->contacts->seltype =seltype;
		}
	}
	for (const auto contact : contactlist->contacts)
	{
		printf("Contact: %s\n", contact->name);
		ContactsEntry *newgui =new ContactsEntry(XwinTox->sidebar->contacts->x(), 
											  XwinTox->sidebar->contacts->y() + YM, 
											  XwinTox->sidebar->contacts->scale, 
											  contact, 0, 0);
		XwinTox->sidebar->contacts->add(newgui);
		XwinTox->sidebar->contacts->entries.push_back(newgui);
		YM += (50 * XwinTox->scale);

		if(!FindContactMArea(contact))
		{
			GMessageArea *newarea =new GMessageArea(XwinTox->sidebar->scale, contact, 0, 0);
			newarea->hide();
			newarea->send->callback(&SendMessagePressed);

			XwinTox->add(newarea);
			XwinTox->contents->messageareas.push_back(newarea);
		}

		if(contact->num == selected && seltype == 0)
		{
			newgui->selected =1;
			XwinTox->contents->NewCurrentArea(FindContactMArea(contact));
			XwinTox->sidebar->contacts->selected =selected;
			XwinTox->sidebar->contacts->seltype =seltype;
		}
	}
	XwinTox->redraw(); XwinTox->sidebar->contacts->redraw();
}

ContactsEntry *FindContactEntry(unsigned int num)
{
	for (const auto entry : XwinTox->sidebar->contacts->entries)
	{
		if(entry->contact->num == num) return entry;
	}
	return 0;
}

GMessageArea *FindContactMArea(Contact_t *contact)
{
	for (const auto messagearea : XwinTox->contents->messageareas)
	{
		if(messagearea->contact == contact) return messagearea;
	}
	return 0;
}

GMessageArea *FindContactMArea(unsigned int num)
{
	for (const auto messagearea : XwinTox->contents->messageareas)
	{
		if(messagearea->contact->num == num) return messagearea;
	}
	return 0;
}

Contact_t *FindContact(unsigned int id)
{
	for (const auto contact : contactlist->contacts)
	{
		if(contact->num == id) return contact;
	}
	return 0;
}

Groupchat_t *FindGroupchat(unsigned int id)
{
	for (const auto contact : groupchats)
	{
		if(contact->num == id) return contact;
	}
	return 0;
}

GMessageArea *FindGroupchatMArea(Groupchat_t *contact)
{
	for (const auto messagearea : XwinTox->contents->messageareas)
	{
		if(messagearea->groupchat == contact) return messagearea;
	}
	return 0;
}

GMessageArea *FindGroupchatMArea(unsigned int num)
{
	for (const auto messagearea : XwinTox->contents->messageareas)
	{
		if(messagearea->groupchat->num == num) return messagearea;
	}
	return 0;
}
