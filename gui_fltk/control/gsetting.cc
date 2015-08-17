#include <FL/Fl.H>
#include <FL/fl_draw.H>
#include <FL/Fl_Tabs.H>

#include "misc.h"
#include "postbox.h"

#include "control/xwin.h"
#include "control/gsetting.h"
#include "control/sr_table.h"

#define TabLabelSize 13.5 * scale

static const char * G_header[4] = {"Module", "Type", "Classes", 0};
static const char * C_header[4] = {"Class", "Language", "Instances", 0};

void ModCellCallback (MyTable * w, void * rowdata, void * userdata)
{
    GSettings * self = static_cast< GSettings * > (userdata);
    List_t * lstClasses = static_cast< List_t * > (self->hObj_->pSvcs->fnCall (
        self->hObj_, "/GETMODULECLASSESINFO", rowdata));

    self->tblClasses->clearall ();

    LIST_ITERATE_OPEN (lstClasses)
    XWF_ClassInfo_t * modinfoCur = static_cast< XWF_ClassInfo_t * > (e_data);
    vector< const char * > rows;

    rows.push_back (modinfoCur->pszName);
    rows.push_back (modinfoCur->pszLang);
    self->tblClasses->load_command (0, rows);
    XWF_FreeClassInfo (modinfoCur);
    LIST_ITERATE_CLOSE (lstClasses)
    free (lstClasses);
    return;
}

GSettings::GSettings (const XWF_hObj_t * hObj, int S)
    : GArea (hObj, S, "Settings")
{
    hObj_ = hObj;
    scale = S;

    pages = new Fl_Tabs (0, 0, 1, 1);
    {
        /* pages begin */
        /* pagePlugins begin */
        pagePlugins = new Fl_Group (0, 0, 1, 1, "Modules");
        {
            List_t * lstMods = static_cast< List_t * > (
                hObj_->pSvcs->fnCall (hObj_, "/GETMODULESINFO", 0));
            tblMods = new MyTable (0, 0, 1, 1, scale, G_header);
            tblMods->selection_color (FL_YELLOW);
            tblMods->col_header (1);
            tblMods->col_resize (1);
            tblMods->when (FL_WHEN_RELEASE); // handle tblMods events on release
            LIST_ITERATE_OPEN (lstMods)
            XWF_ModInfo_t * modinfoCur =
                static_cast< XWF_ModInfo_t * > (e_data);
            vector< const char * > rows;

            rows.push_back (modinfoCur->pszName);
            rows.push_back (modinfoCur->pszType);
            rows.push_back (modinfoCur->pszClasses);
            tblMods->load_command (modinfoCur->hMod, rows);
            XWF_FreeModInfo (modinfoCur);
            LIST_ITERATE_CLOSE (lstMods)
            free (lstMods);
            tblMods->col_header_height (18 * scale); // height of headers
            tblMods->row_height_all (16 * scale);    // height of all rows
            tblMods->setcellcallback (ModCellCallback, this);

            tblClasses = new MyTable (0, 0, 1, 1, scale, C_header);
            tblClasses->selection_color (FL_YELLOW);
            tblClasses->col_header (1);
            tblClasses->col_resize (1);
            tblClasses->when (FL_WHEN_RELEASE); // handle events on release
            tblClasses->col_header_height (18 * scale); // height of headers
            tblClasses->row_height_all (16 * scale);    // height of all rows
        }
        pagePlugins->labelsize (TabLabelSize);
        pagePlugins->end ();
        /* pagePlugins end */
        pageIdentity = new Fl_Group (0, 0, 1, 1, "Identity");
        pageIdentity->labelsize (TabLabelSize);
        pageIdentity->end ();
    }
    pages->end ();
    end ();
    resize (x (), y (), w (), h ());
}

void GSettings::resize (int X, int Y, int W, int H)
{
    Fl_Group::resize (X, Y, W, H);
    pages->resize (x () + (5 * scale), y () + (65 * scale),
                   x () + w () - (234 * scale), y () + h () - (105 * scale));

    pagePlugins->resize (pages->x (), pages->y () + (22.5 * scale), pages->w (),
                         pages->h () - (22.5 * scale));
    /* pagePlugins */
    int ppY = pages->y () + (27.5 * scale), ppX = pages->x () + (5 * scale);
    int ppW = pages->w () - (10 * scale), ppH = pages->h () - (32.5 * scale);
    tblMods->resize (ppX, ppY, ppW, ppH / 2 - (20 * scale));
    tblClasses->resize (ppX, ppY + (ppH / 2 - (10 * scale)), ppW,
                        ppH / 2 - (20 * scale));
    pageIdentity->resize (pages->x (), pages->y () + (22.5 * scale),
                          pages->w (), pages->h () - (22.5 * scale));
}

void GSettings::draw () { GArea::draw (); }
