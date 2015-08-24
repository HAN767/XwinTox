#include <cstdio>
#include <cstdlib>
#include <string>

#include "xwfplatf.h"

#ifdef HAVE_UNISTD_H
#include <unistd.h>
#else
#include "getopt.h"
#endif

#include "xwfvm.h"

using namespace std;

int main (int argc, char * argv[])
{
    extern char * optarg;
    extern int optind;
    int c, err = 0;
    char * libsdir = 0;
    static char usage[] = "usage: %s -l libsdir\n";
    char buffer[256];
    int returnValue = 0;

    while ((c = getopt (argc, argv, ":l:")) != -1)
        switch (c)
        {
        case 'l':
            libsdir = optarg;
            break;

        case '?':
            err = 1;
            break;
        }
    if (libsdir == 0)
    {
        fprintf (stderr, "%s: missing -l option (specifies libdir)\n", argv[0]);
        fprintf (stderr, usage, argv[0]);
        exit (1);
    }
    else if (err)
    {
        fprintf (stderr, usage, argv[0]);
        exit (1);
    }

    VM newVm (libsdir);

    printf ("XwinFast QuickSilver(TM) Performance Engine [1.98 DR0]\n");
    printf ("[" XWF_COMPILER_INFO " on " XWF_SYSTEM_INFO ", " __DATE__
            ", at " __TIME__ "]\n");

    while (returnValue != 99)
    {
        std::string line;
        std::string prompt ("{" + iToA (newVm.OStack.size ()) + "} ok ");
        fputs (prompt.c_str (), stdout);
        fflush (stdout);
        if (fgets (buffer, sizeof (buffer), stdin) == NULL)
            break;

        newVm.Eval (buffer);
    }
    return 0;
}
