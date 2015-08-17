#include <FL/Fl.H>
#include <FL/Fl_Scroll.H>

#include "misc.h"

#include "control/translst.h"
#include "control/transent.h"

TransfersList::TransfersList (int X, int Y, int W, int H, int S)
    : Fl_Scroll (X, Y, W, H)
{
    scale = S;
    startpoint = 0 * S;

    type (VERTICAL_ALWAYS);
    end ();
}

void TransfersList::draw ()
{
    Fl_Scroll::draw ();

    for (const auto entry : entries)
    {
        entry->redraw ();
    }
}

void TransfersList::resize (int X, int Y, int W, int H)
{
    Fl_Scroll::resize (X, Y, W, H);

    for (const auto entry : entries)
    {
        entry->resize (entry->x (), entry->y (), W, entry->h ());
        entry->redraw ();
    }
}

int TransfersList::handle (int event)
{
    switch (event)
    {
    case FL_PUSH:
        if (Fl::event_button () == FL_LEFT_MOUSE)
        {
            for (const auto entry : entries)
            {
                entry->redraw ();
            }
        }
    }

    return Fl_Scroll::handle (event);
}

void TransfersList::regen_gui ()
{
    int YM = y (), XM = x (), inv = 0;

    for (const auto entry : entries)
    {
        entry->inv = inv;
        entry->position (XM, YM);
        dbg ("Regen for Trans\n");

        entry->show ();
        YM += 50;
        inv = 1 - inv;
    }

    redraw ();

    return;
}

void TransfersList::clear_all ()
{
    // for (const auto entry: entrie)
    this->redraw ();
    parent ()->redraw ();
}
