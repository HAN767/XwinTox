#include <Object.h>
#include "Fl_Types.h"
#include "Fl_C.h"
#include "Fl_WindowC.h"

int main ()
{
    fl_Window win = (fl_Window)Fl_Window_New_WithLabel (640, 480, "XwinTox");
    Fl_check ();
    Fl_Window_show (win);
    return Fl_run ();
}