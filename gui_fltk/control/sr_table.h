#ifndef __SR_TABLE_H_
#define __SR_TABLE_H

#include <FL/Fl_Browser.H>
#include <FL/fl_draw.H>

class ColResizeBrowser : public Fl_Browser
{
	Fl_Color  _colsepcolor;     // color of column separator lines
	int       _showcolsep;      // flag to enable drawing column separators
	Fl_Cursor _last_cursor;     // saved cursor state info
	int       _dragging;        // 1=user dragging a column
	int       _dragcol;         // col# user is currently dragging
	int      *_widths;          // pointer to user's width[] array
	int       _nowidths[1];     // default width array (non-const)
	// CHANGE CURSOR
	//     Does nothing if cursor already set to value specified.
	//
	void change_cursor(Fl_Cursor newcursor)
	{
		if(newcursor != _last_cursor)
		{
			fl_cursor(newcursor, FL_BLACK, FL_WHITE);
			_last_cursor = newcursor;
		}
	}
	// RETURN THE COLUMN MOUSE IS 'NEAR'
	//     Returns -1 if none.
	//
	int which_col_near_mouse()
	{
		int X,Y,W,H;
		Fl_Browser::bbox(X,Y,W,H);            // area inside browser's box()

		// EVENT NOT INSIDE BROWSER AREA? (eg. on a scrollbar)
		if(! Fl::event_inside(X,Y,W,H))
		{
			return(-1);
		}

		int mousex = Fl::event_x() + hposition();
		int colx = this->x();

		for(int t=0; _widths[t]; t++)
		{
			colx += _widths[t];
			int diff = mousex - colx;

			// MOUSE 'NEAR' A COLUMN?
			//     Return column #
			//
			if(diff >= -4 && diff <= 4)
			{
				return(t);
			}
		}

		return(-1);
	}
protected:
	// MANAGE EVENTS TO HANDLE COLUMN RESIZING
	int handle(int e)
	{
		// Not showing column separators? Use default Fl_Browser::handle() logic
		if(! showcolsep()) return(Fl_Browser::handle(e));

		// Handle column resizing
		int ret = 0;

		switch(e)
		{
		case FL_ENTER:
		{
			ret = 1;
			break;
		}

		case FL_MOVE:
		{
			if(which_col_near_mouse() >= 0)
			{
				change_cursor(FL_CURSOR_WE);
			}
			else
			{
				change_cursor(FL_CURSOR_DEFAULT);
			}

			ret = 1;
			break;
		}

		case FL_PUSH:
		{
			int whichcol = which_col_near_mouse();

			if(whichcol >= 0)
			{
				// CLICKED ON RESIZER? START DRAGGING
				ret = 1;
				_dragging = 1;
				_dragcol = whichcol;
				change_cursor(FL_CURSOR_DEFAULT);
			}

			break;
		}

		case FL_DRAG:
		{
			if(_dragging)
			{
				ret = 1;
				// Sum up column widths to determine position
				int mousex = Fl::event_x() + hposition();
				int newwidth = mousex - x();

				for(int t=0; _widths[t] && t<_dragcol; t++)
				{
					newwidth -= _widths[t];
				}

				if(newwidth > 0)
				{
					// Apply new width, redraw interface
					_widths[_dragcol] = newwidth;

					if(_widths[_dragcol] < 2)
					{
						_widths[_dragcol] = 2;
					}

					redraw();
				}
			}

			break;
		}

		case FL_LEAVE:
		case FL_RELEASE:
		{
			_dragging = 0;                          // disable drag mode
			change_cursor(FL_CURSOR_DEFAULT);       // ensure normal cursor
			ret = 1;
			break;
		}
		}

		if(_dragging) return(1);                        // dragging? don't pass event to Fl_Browser

		return(Fl_Browser::handle(e) ? 1 : ret);
	}
	void draw()
	{
		// DRAW BROWSER
		Fl_Browser::draw();

		if(_showcolsep)
		{
			// DRAW COLUMN SEPARATORS
			int colx = this->x() - hposition();
			int X,Y,W,H;
			Fl_Browser::bbox(X,Y,W,H);
			fl_color(_colsepcolor);

			for(int t=0; _widths[t]; t++)
			{
				colx += _widths[t];

				if(colx > X && colx < (X+W))
				{
					fl_line(colx, Y, colx, Y+H-1);
				}
			}
		}
	}
public:
	// CTOR
	ColResizeBrowser(int X,int Y,int W,int H,const char*L=0) : Fl_Browser(X,Y,W,H,L)
	{
		_colsepcolor = Fl_Color(FL_GRAY);
		_last_cursor = FL_CURSOR_DEFAULT;
		_showcolsep  = 0;
		_dragging    = 0;
		_nowidths[0] = 0;
		_widths      = _nowidths;
	}
	// GET/SET COLUMN SEPARATOR LINE COLOR
	Fl_Color colsepcolor() const
	{
		return(_colsepcolor);
	}
	void colsepcolor(Fl_Color val)
	{
		_colsepcolor = val;
	}
	// GET/SET DISPLAY OF COLUMN SEPARATOR LINES
	//     1: show lines, 0: don't show lines
	//
	int showcolsep() const
	{
		return(_showcolsep);
	}
	void showcolsep(int val)
	{
		_showcolsep = val;
	}
	// GET/SET COLUMN WIDTHS ARRAY
	//    Just like fltk method, but array is non-const.
	//
	int *column_widths() const
	{
		return(_widths);
	}
	void column_widths(int *val)
	{
		_widths = val;
		Fl_Browser::column_widths(val);
	}
};


#endif
