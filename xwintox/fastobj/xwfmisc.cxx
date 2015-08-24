#include <string>
#include <cstdio>

#include "xwfplatf.h"

using namespace std;

string iToA (XwfI Number)
{
    char buf[255];
    sprintf (buf, "%ld", Number);
    return string (buf);
}

string iToA (XwfU Number)
{
    char buf[255];
    sprintf (buf, "%lu", Number);
    return string (buf);
}

string iToA (double Number)
{
    char buf[255];
    sprintf (buf, "%f", Number);
    return string (buf);
}
