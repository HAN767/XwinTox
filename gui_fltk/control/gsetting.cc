#include <FL/Fl.H>
#include <FL/fl_draw.H>
#include <FL/Fl_Tabs.H>

#include "misc.h"
#include "postbox.h"

#include "control/xwin.h"
#include "control/gsetting.h"
#include "control/sr_table.h"

GSettings::GSettings(const XWF_hObj_t* hObj, int S) :
	GArea(hObj, S, "Settings")
{
	scale = S;

	pages =new Fl_Tabs(0, 0, 1, 1);
	{
		/* pages begin */
		/* pagePlugins begin */
		pagePlugins =new Fl_Group(0, 0, 1, 1, "Modules");
		{
			tblMods =new MyTable(0, 0, 1, 1);
			tblMods->selection_color(FL_YELLOW);
			tblMods->col_header(1);
			tblMods->col_resize(1);
			tblMods->when(FL_WHEN_RELEASE); // handle tblMods events on release
			tblMods->row_height_all(18); // height of all rows
			tblMods->load_command("Test","Test");
			tblMods->load_command("Test2","Test Two");

			/*b->showcolsep(1);
			b->colsepcolor(FL_RED);
			b->column_char('\t'); // tabs as column delimiters
			b->type(FL_MULTI_BROWSER);

			// NICER COLORED HEADING
			b->add("@B12@C7@b@.Module\tDescription");
			b->add("Test Module\tDoes a test.");
			b->add("Test Module\tdoes many many many tests");
			b->end();
			tableMods =b;*/
		}
		pagePlugins->end();
		/* pagePlugins end */
		pageIdentity =new Fl_Group(0, 0, 1, 1, "Identity");
		pageIdentity->end();
	}
	pages->end();
	end();
	resize(x(), y(), w(), h());
}

void GSettings::resize(int X, int Y, int W, int H)
{
	Fl_Group::resize(X, Y, W, H);
	pages->resize(x() + (5 * scale), y() + (65 * scale),
	              x() + w() - (234 * scale), y() + h() - (105 * scale));

	pagePlugins->resize(pages->x(), pages->y() + (22.5 * scale), pages->w(),
	                    pages->h() - (22.5 * scale));
	/* pagePlugins */
	int ppY =pages->y() + (27.5 * scale), ppX = pages->x() + (5 * scale);
	int ppW =pages->w() - (10 * scale), ppH =pages->h() - (32.5 * scale);
	tblMods->resize(ppX, ppY, ppW, ppH);
	pageIdentity->resize(pages->x(), pages->y() + (22.5 * scale), pages->w(),
	                     pages->h() - (22.5 * scale));
}

void GSettings::draw()
{
	GArea::draw();
}
