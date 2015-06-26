#ifndef CNTCTENT_H_
#define CNTCTENT_H_

#include <FL/Fl_Box.H>

#include "xwin/contacts.h"

class SVGBox;

class ContactsEntry: public Fl_Box
{
public:
	// Ctor
	ContactsEntry(int X, int Y, int S, Contact_t *C, Groupchat_t *G, short T);

	void draw();
	int handle(int event);

	SVGBox *icon;
	SVGBox *invicon;

	short type; /* 0 = contact, 1 = groupchat */
	Contact_t *contact;
	Groupchat_t *groupchat;
	int selected, scale;
};

#endif
