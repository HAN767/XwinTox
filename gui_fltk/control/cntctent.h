#ifndef CNTCTENT_H_
#define CNTCTENT_H_

#include <FL/Fl_Group.H>

#include "xwintox.h"

class SVGBox;

class ContactsEntry : public Fl_Group
{
  public:
    // Ctor
    ContactsEntry (const XWF_hObj_t * hObj, int X, int Y, int S,
                   XWContact_t * C, XWGroupchat_t * G, short T);

    void draw ();
    int handle (int event);

    SVGBox * icon;
    SVGBox * invicon;

    short type; /* 0 = contact, 1 = groupchat */
    XWContact_t * contact;
    XWGroupchat_t * groupchat;
    int selected, scale;

    const XWF_hObj_t * hObj_;
};

#endif
