#ifndef MSGAREA_H_
#define MSGAREA_H_

#include <FL/Fl_Button.H>
#include <FL/Fl_Group.H>
#include <FL/Fl_Multiline_Output.H>
#include <FL/Fl_Text_Display.H>
#include <FL/Fl_Select_Browser.H>

#include "control/svgbox.h"

class GMessageArea : public Fl_Group
{
  public:
    // Ctor
    GMessageArea (const XWF_hObj_t * hObj, int S, XWContact_t * C,
                  XWGroupchat_t * G, short T);

    void resize (int X, int Y, int W, int H);
    void draw ();

    SVGBox * icon;
    Fl_Multiline_Output * gnames;
    Fl_Select_Browser * names;
    Fl_Text_Display * moutput;
    Fl_Text_Buffer * moutbuffer;
    Fl_Input * message;
    SVGBox * attach;
    SVGBox * emoji;
    Fl_Button * send;

    int scale;
    short mtype;
    XWContact_t * contact;
    XWGroupchat_t * groupchat;
    const XWF_hObj_t * hObj_;
};

#endif
