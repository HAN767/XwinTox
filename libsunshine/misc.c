/*
 * LibSunshine
 *
 * Miscellaneous utility functionality
 */

#include <errno.h>
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <pwd.h>
#include <unistd.h>

#include <threads.h>

#define DEF_EV
#include "misc.h"
#include "list.h"

List_t * lstLogs = 0;

/* Debugging functionality */
void dbg2 (const char * funcname, const char * format, ...)
{
    // char * start, *finish, *comb;
    va_list arglist;
    va_start (arglist, format);
    /*if (!lstLogs)
        lstLogs = List_new ();
    asprintf (&start, KRED "[dbg] " KMAG "[%s] " KNRM, funcname);
    vasprintf (&finish, format, arglist);
    comb = malloc (strlen (start) + strlen (finish));
    strcpy (comb, start);
    strcpy (comb + strlen (start), finish);
    List_add (lstLogs, comb);
    free (start);
    free (finish);*/
    printf (KRED "[dbg] " KMAG "[%s] " KNRM, funcname);
    vprintf (format, arglist);
    va_end (arglist);
}

void dbgPrint ()
{
    LIST_ITERATE_OPEN (lstLogs);
    printf ("%s", e_data);
    free (e_data);
    free (e);
    LIST_ITERATE_CLOSE (lstLogs);
    lstLogs->List = 0;
}

/* Files */
int create_folder_if_not_exist (const char * path)
{
    int error;

    if ((error = mkdir (path, 0700)) == -1)
    {
        struct stat s;

        if (errno == EEXIST && !stat (path, &s) && s.st_mode & S_IFDIR)
        {
            return 0;
        }
        else
        {
            dbg ("Failed to create config folder: %s", strerror (errno));
            return 1;
        }
    }

    return 0;
}

const char * get_home_folder ()
{
    const char * homefolder;

    if ((homefolder = getenv ("HOME")) == NULL)
    {
        homefolder = getpwuid (getuid ())->pw_dir;
    }

    return homefolder;
}
