#ifndef TRANSLST_H_
#define TRANSLST_H_

#include <vector>

using namespace std;

#include <FL/Fl_Button.H>
#include <FL/Fl_Scroll.H>

#include "control/transent.h"

class TransfersEntry;

class TransfersList : public Fl_Scroll
{
  public:
    // Ctor
    TransfersList (int X, int Y, int W, int H, int S);

    void draw ();
    void resize (int X, int Y, int W, int H);
    int handle (int event);

    void regen_gui ();
    void clear_all ();

    vector< TransfersEntry * > entries;

    /* the purpose of 'startpoint' is to indicate where
     * the first ContactEntry should be drawn relative to
     * the y() of the contacts list*/
    int scale, startpoint;
};

#endif
