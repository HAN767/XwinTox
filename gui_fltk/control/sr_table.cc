#include "control/sr_table.h"

// Sort a column up or down
void MyTable::sort_column(int col, int reverse)
{
	std::sort(_rowdata.begin(), _rowdata.end(), SortColumn(col, reverse));
	redraw();
}
// Draw sort arrow
void MyTable::draw_sort_arrow(int X,int Y,int W,int H)
{
	int xlft = X+(W-6)-8;
	int xctr = X+(W-6)-4;
	int xrit = X+(W-6)-0;
	int ytop = Y+(H/2)-4;
	int ybot = Y+(H/2)+4;

	if(_sort_reverse)
	{
// Engraved down arrow
		fl_color(FL_WHITE);
		fl_line(xrit, ytop, xctr, ybot);
		fl_color(41); // dark gray
		fl_line(xlft, ytop, xrit, ytop);
		fl_line(xlft, ytop, xctr, ybot);
	}
	else
	{
// Engraved up arrow
		fl_color(FL_WHITE);
		fl_line(xrit, ybot, xctr, ytop);
		fl_line(xrit, ybot, xlft, ybot);
		fl_color(41); // dark gray
		fl_line(xlft, ybot, xctr, ytop);
	}
}
// Handle drawing all cells in table
void MyTable::draw_cell(TableContext context, int R, int C, int X, int Y, int W, int H)
{
	const char *s = "";

	if(R < (int)_rowdata.size() && C < (int)_rowdata[R].cols.size())
		s = _rowdata[R].cols[C];

	switch(context)
	{
	case CONTEXT_COL_HEADER:
		fl_push_clip(X,Y,W,H);
		{
			fl_draw_box(FL_THIN_UP_BOX, X,Y,W,H, FL_BACKGROUND_COLOR);

			if(C < 9)
			{
				fl_font(HEADER_FONTFACE, HEADER_FONTSIZE);
				fl_color(FL_BLACK);
				fl_draw(G_header[C], X+2,Y,W,H, FL_ALIGN_LEFT, 0, 0); // +2=pad left

// Draw sort arrow
				if(C == _sort_lastcol)
				{
					draw_sort_arrow(X,Y,W,H);
				}
			}
		}
		fl_pop_clip();
		return;

	case CONTEXT_CELL:
	{
		fl_push_clip(X,Y,W,H);
		{
// Bg color
			Fl_Color bgcolor = row_selected(R) ? selection_color() : FL_WHITE;
			fl_color(bgcolor);
			fl_rectf(X,Y,W,H);
			fl_font(ROW_FONTFACE, ROW_FONTSIZE);
			fl_color(FL_BLACK);
			fl_draw(s, X+2,Y,W,H, FL_ALIGN_LEFT); // +2=pad left
// Border
			fl_color(FL_LIGHT2);
			fl_rect(X,Y,W,H);
		}
		fl_pop_clip();
		return;
	}

	default:
		return;
	}
}
// Automatically set column widths to widest data in each column
void MyTable::autowidth(int pad)
{
	int w, h;
// Initialize all column widths to header width
	fl_font(HEADER_FONTFACE, HEADER_FONTSIZE);

	for(int c=0; G_header[c]; c++)
	{
		w=0;
		fl_measure(G_header[c], w, h, 0); // pixel width of header text
		col_width(c, w+pad);
	}

	fl_font(ROW_FONTFACE, ROW_FONTSIZE);

	for(int r=0; r<(int)_rowdata.size(); r++)
	{
		for(int c=0; c<(int)_rowdata[r].cols.size(); c++)
		{
			w=0;
			fl_measure(_rowdata[r].cols[c], w, h, 0); // pixel width of row text

			if((w + pad) > col_width(c)) col_width(c, w + pad);
		}
	}

	table_resized();
	redraw();
}

// Load table with output of 'cmd'
void MyTable::load_command(void *userdata, std::vector <const char*> columns)
{
	cols(0);
// Add a new row
	Row newrow;

	newrow.userdata =userdata;
	_rowdata.push_back(newrow);
	std::vector<char*> &rc = _rowdata[_rowdata.size() -1].cols;

	for (const char* column : columns)
	{
		rc.push_back(strdup(column));
	}

	/*rc.push_back(strdup(t1));
	rc.push_back(strdup(t2));*/

// Keep track of max # columns
	if((int)rc.size() > cols())
	{
		cols((int)rc.size());
	}


// How many rows we loaded
	rows((int)_rowdata.size());
// Auto-calculate widths, with 20 pixel padding
	autowidth(20 * scale);
}
// Callback whenever someone clicks on different parts of the table
void MyTable::event_callback(Fl_Widget*, void *data)
{
	MyTable *o = (MyTable*)data;
	o->event_callback2();
}
void MyTable::event_callback2()
{
//int ROW = callback_row(); // unused
	int COL = callback_col();
	TableContext context = callback_context();

	switch(context)
	{
	case CONTEXT_COL_HEADER:   // someone clicked on column header
	{
		if(Fl::event() == FL_RELEASE && Fl::event_button() == 1)
		{
			if(_sort_lastcol == COL)      // Click same column? Toggle sort
			{
				_sort_reverse ^= 1;
			}
			else     // Click diff column? Up sort
			{
				_sort_reverse = 0;
			}

			sort_column(COL, _sort_reverse);
			_sort_lastcol = COL;
		}

		break;
	}
	case CONTEXT_CELL:
	{
		if(Fl::event() == FL_PUSH && Fl::event_button() == 1)
		{
			int rownum =callback_row();
			char *text =_rowdata[rownum].cols[0];
			printf("Context: Row %d. Text: %s\n", rownum, text);
			cellcallback(this, _rowdata[rownum].userdata, userdatacellcallback);
		}
	}

	default:
		return;
	}
}
