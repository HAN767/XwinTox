#ifndef FREQUEST_H_
#define FREQUEST_H_

#include <vector>
using namespace std;

#include <FL/Fl_Box.H>
#include <FL/Fl_Button.H>
#include <FL/Fl_Multiline_Output.H>

#include "xwintox.h"

const int frheight = 88;

class FriendRequests : public Fl_Box
{
  public:
    // Ctor
    FriendRequests (const XWF_hObj_t * hobj, int X, int Y, int S);

    void show ();
    void hide ();
    void draw ();
    void resize (int X, int Y, int W, int H);
    /*int handle(int event);*/

    Fl_Output * address;
    Fl_Multiline_Output * message;
    Fl_Button * accept, *reject, *next;

    vector< XWFriendRequest_t * > frs;
    int selected, scale;

    const XWF_hObj_t * hObj_;
};

#endif
