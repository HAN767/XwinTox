#include <algorithm>
#include <vector>

#include <FL/Fl_PNG_Image.H>
#include <FL/Fl_Box.H>
#include <FL/Fl_Menu.H>

#include "nanosvg/svgs.h"
#include "xwintox.h"

#include "control/xwin.h"
#include "control/msgarea.h"
#include "control/cntctlst.h"
#include "control/sidebar.h"
#include "control/svgbox.h"
#include "list.h"
#include "util.h"
#include "misc.h"

void ctentFrStatus (int, PBMessage_t * msg, void * custom)
{
    ContactsEntry * self = static_cast< ContactsEntry * > (custom);

    if ((msg->I1) != self->contact->wNum)
        return;

    Fl::lock ();
    self->contact->wStatus = msg->I2;
    self->redraw ();
    Fl::unlock ();
    Fl::awake ();

    PBMessage_t * msgNew = PB_New_Message (); /* periodic save data */
    self->hObj_->pSvcs->fnDispatch (self->hObj_, clSaveData, msgNew);
}

void ctentFrName (int, PBMessage_t * msg, void * custom)
{
    ContactsEntry * self = static_cast< ContactsEntry * > (custom);

    if ((msg->I1) != self->contact->wNum)
        return;

    Fl::lock ();

    if (self->contact->pszName)
        free (self->contact->pszName);

    self->contact->pszName = strdup (msg->S1);
    self->redraw ();
    Fl::unlock ();
    Fl::awake ();
}

void ctentFrAvatar (int, PBMessage_t * msg, void * custom)
{
    ContactsEntry * self = static_cast< ContactsEntry * > (custom);

    printf ("%d == %d\n", msg->I1, self->contact->wNum);

    if (msg->I1 == self->contact->wNum)
    {
        Fl_PNG_Image rawImage (msg->S1);
        Fl_RGB_Image * newImage = dynamic_cast< Fl_RGB_Image * > (
                           rawImage.copy (40 * self->scale, 40 * self->scale)),
                       * newImage2 =
                           dynamic_cast< Fl_RGB_Image * > (newImage->copy ());
        delete (self->icon->img);
        delete (self->invicon->img);
        self->icon->img = newImage;
        self->invicon->img = newImage2;
        self->redraw ();
        return;
    }
}

void ce_deletecontact (ContactsEntry * ce)
{
    /* ugly but necessary */
    userdata * ud = new userdata_t;
    ud->op = 'U';

    PBMessage_t * delMsg = PB_New_Message ();
    vector< ContactsEntry * > * ref = &((ContactsList *)ce->parent ())->entries;
    ContactsList * ctLst = (ContactsList *)ce->parent ();
    ud->lptr = ctLst;

    XwinTox * Xw = getXwin (ce);
    vector< GMessageArea * > * mareaVec = &Xw->contents->messageareas;
    GMessageArea * mareaToDel = 0;

    ref->erase (std::remove (ref->begin (), ref->end (), ce), ref->end ());

    if (ce->type == 0)
    {
        mareaToDel = FindContactMArea (Xw, ce->contact);
        dbg ("mareaToDel: %p\n", mareaToDel);
        List_del (ctLst->lstContacts, ce->contact);
        delMsg->I1 = ce->contact->wNum;
        freeContact (ce->contact);
        ce->hObj_->pSvcs->fnDispatch (ce->hObj_, frDelete, delMsg);
        ce->hObj_->pSvcs->fnDesubscribe (ce->hObj_, frStatus, ce,
                                         ctentFrStatus);
        ce->hObj_->pSvcs->fnDesubscribe (ce->hObj_, frName, ce, ctentFrName);
        ce->hObj_->pSvcs->fnDesubscribe (ce->hObj_, frAvDownloaded, ce,
                                         ctentFrAvatar);
    }

    mareaVec->erase (
        std::remove (mareaVec->begin (), mareaVec->end (), mareaToDel),
        mareaVec->end ());

    if (Xw->contents->currentarea == mareaToDel)
    {
        Xw->contents->NewCurrentArea (Xw->contents->addfriend);
    }

    dbg ("ce: %p\n", ce);
    Fl::delete_widget (ce);
    Fl::delete_widget (mareaToDel);

    Fl::awake (ud);
}

ContactsEntry::ContactsEntry (const XWF_hObj_t * hObj, int X, int Y, int S,
                              XWContact_t * C, XWGroupchat_t * G, short T)
    : Fl_Group (X, Y, 224 * S, 50 * S), hObj_ (hObj), scale (S), contact (C),
      groupchat (G), type (T), selected (0)
{
    if (!T) /* regular contact */
    {
        icon = new SVGBox (X + (4 * S), Y + (2 * S), 40 * S, 40 * S, S,
                           default_av, 1);
        invicon = new SVGBox (X + (4 * S), Y + (2 * S), 40 * S, 40 * S, S,
                              default_av, 1);
        invicon->hide ();
        groupchat = new XWGroupchat_t;
        groupchat->wNum = 65535;
        hObj_->pSvcs->fnSubscribe (hObj_, frStatus, this, ctentFrStatus);
        hObj_->pSvcs->fnSubscribe (hObj_, frName, this, ctentFrName);
        hObj_->pSvcs->fnSubscribe (hObj_, frAvDownloaded, this, ctentFrAvatar);
    }
    else
    {
        icon = new SVGBox (X - (8 * S), Y + (2 * S), 40 * S, 40 * S, S,
                           groupsvg, 0.63);
        invicon = new SVGBox (X + (14 * S), Y + (12 * S), 40 * S, 40 * S, S,
                              groupsvg2, 0.63);
        invicon->hide ();
        contact = new XWContact_t;
        contact->wNum = 65535;
        contact->blAvatar = false;
    }

    icon->box (FL_ENGRAVED_BOX);
    icon->color (fl_rgb_color (130, 128, 133));

    invicon->box (FL_ENGRAVED_BOX);
    invicon->color (fl_rgb_color (130, 128, 133));

    box (FL_FLAT_BOX);
    color (fl_rgb_color (65, 65, 65));
    end ();
}

void ContactsEntry::draw ()
{
    int txt_color = 255;
    char * name, *status;

    fl_push_clip (x (), y (), w (), h ());

    if (selected)
    {
        color (255);
        txt_color = 0;
        icon->hide ();
        invicon->show ();
    }
    else
    {
        color (fl_rgb_color (65, 65, 65));
        txt_color = 255;
        invicon->hide ();
        icon->show ();
    }

    Fl_Group::draw ();

    if (!type) /* todo: resize() calls to the svgbox, make it work */
    {
        name = GetDisplayName (contact, 16);
        status = GetDisplayStatus (contact, 23);

        if (contact->wStatus == stOnline)
            fl_color (2);
        else if (contact->wStatus == stBusy | contact->wStatus == stAway)
        {
            fl_color (FL_YELLOW);
        }
        else
            fl_color (FL_RED);

        fl_pie (x () + (185 * scale), this->y () + (20 * scale), 10 * scale,
                10 * scale, 0, 360);
        icon->resize (x () + (5 * scale), y () + (5 * scale), 40 * scale,
                      40 * scale);
        invicon->resize (x () + (5 * scale), y () + (5 * scale), 40 * scale,
                         40 * scale);
    }
    else
    {
        name = groupchat->pszName;
        status = (char *)"";

        icon->resize (x () - (8 * scale), y () + (2 * scale), 40 * scale,
                      40 * scale);
        invicon->resize (x () + (14 * scale), y () + (12 * scale), 40 * scale,
                         40 * scale);
    }

    fl_color (txt_color);
    fl_font (FL_HELVETICA, 12 * scale);
    fl_draw (name, x () + (54 * scale), y () + (22 * scale));
    fl_font (FL_HELVETICA, 10 * scale);
    fl_draw (status, x () + (54 * scale), y () + (36 * scale));

    fl_pop_clip ();
}

int ContactsEntry::handle (int event)
{
    switch (event)
    {
    case FL_PUSH:
        if (Fl::event_button () == FL_LEFT_MOUSE)
        {
            for (const auto entry : ((ContactsList *)parent ())->entries)
            {
                entry->selected = 0;
                entry->redraw ();
                entry->icon->redraw ();
                entry->invicon->redraw ();
                ((Sidebar *)(parent ()->parent ()))
                    ->bottom_area->deselect_all ();
            }

            selected = 1;
            ((ContactsList *)parent ())->seltype = type;
            redraw ();
            icon->redraw ();
            invicon->redraw ();

            if (!type)
            {
                ((ContactsList *)parent ())->selected = contact->wNum;
            }
            else
            {
                ((ContactsList *)parent ())->selected = groupchat->wNum;
            }

            icon->hide ();
            invicon->show ();

            if (!type)
            {
                ((ContactsList *)parent ())
                    ->Xw->contents->NewCurrentArea (FindContactMArea (
                        ((ContactsList *)parent ())->Xw, contact));
            }
            else
            {
                // Xw->contents->NewCurrentArea(Xw,
                // FindGroupchatMArea(groupchat));
            }

            return 1;
        }
        else if (Fl::event_button () == FL_RIGHT_MOUSE)
        {
            Fl_Menu_Item contmenu[] = {{"Delete"}, {0}};
            contmenu->labelsize (12 * scale);
            const Fl_Menu_Item * m =
                contmenu->popup (Fl::event_x (), Fl::event_y (), 0, 0, 0);

            if (!m)
                return 0;
            else if (strcmp (m->label (), "Delete") == 0)
            {
                ce_deletecontact (this);
                return 1;
            }
        }
    }

    return 0;
}
