#include <fstream>
#include <streambuf>

#include "xwfvm.h"

static void removeAngles (std::string & x)
{
    for (std::string::iterator iter = x.begin (); iter != x.end ();)
    {
        if (*iter == '<' || *iter == '>')
            iter = x.erase (iter);
        else
            ++iter;
    }
}

int VM::LoadFile (std::string name)
{
    std::ifstream ifsSoftcore;
    std::string strSoftcore;
    removeAngles (name);
    std::string locSoftCore = libPath + "/" + name + ".fst";

    printf ("Loading <%s>...", name.c_str ());
    ifsSoftcore.open ((locSoftCore.c_str ()), std::ios::in);
    if (!ifsSoftcore)
    {
        printf ("File %s doesn't exist.\n", locSoftCore.c_str ());
        return 1;
    }
    ifsSoftcore.seekg (0, std::ios::end);
    strSoftcore.resize ((unsigned int)ifsSoftcore.tellg ());
    ifsSoftcore.seekg (0, std::ios::beg);
    ifsSoftcore.read (&strSoftcore[0], strSoftcore.size ());
    ifsSoftcore.close ();
    Eval (strSoftcore.c_str ());
    printf ("\n");
    return 0;
}

void VM::loadSoftKernel ()
{
    if (LoadFile ("kernel"))
        exit (1);
}

void VM::DictionaryFromStack ()
{
    WordPtr newDict = FactoryW.MakeDictionary ("-no-name-");
    StackIter marker = OStack.rbegin ();
    short numToPop = 0;

    while ((*marker)->GetType () != W_MARKER)
    {
        marker++;
        numToPop++;
    }

    for (StackForwardIter iter = marker.base (); iter != OStack.end (); iter++)
        As (WDictionary, &newDict)->AddFromWords (*iter, *(++iter));

    /* Pop the marker and used entries. */
    for (short i = 0; i <= numToPop; i++)
        OPop ();

    /* Now add the newly formed dictionary to the operand stack.
     * It will acquire a unique name when `def' is called on it. */
    OPush (newDict);
}

void VM::ArrayFromStack ()
{
    WordPtr newArray = FactoryW.MakeArray ("-no-name-");
    StackIter marker = OStack.rbegin ();
    short numToPop = 0;

    while ((*marker)->GetType () != W_MARKER)
    {
        printf ("hello\n");
        marker++;
        numToPop++;
    }

    for (StackForwardIter iter = marker.base (); iter != OStack.end (); iter++)
        As (WArray, &newArray)->AddWord (*iter);

    /* Pop the marker and used entries. */
    for (short i = 0; i <= numToPop; i++)
        OPop ();

    /* Now add the newly formed dictionary to the operand stack.
     * It will acquire a unique name when `def' is called on it. */
    OPush (newArray);
}

void VM::ExecuteIdentifier (std::string token)
{
    UpperString (token);
    LookupResult res = DictsLookup (token);
    if (res.first)
        res.second->Execute (*this);
    else
        printf ("Error: unknown identifier: %s\n", token.c_str ());
}
