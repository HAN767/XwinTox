#ifndef __XWF__VM___
#define __XWF__VM___

#include <list>
#include <map>
#include <string>
#include <vector>
#include <cstring>

#include "xwforth.h"
#include "xwfword.h"

struct yyVMData
{
    yyVMData ()
        : strdefsize (32), strcnt (0), strsize (strdefsize), bracketdepth (0)
    {
    }
    int strdefsize;

    char * buf, *s;
    int strcnt, strsize, bracketdepth;

    void init ()
    {
        buf = (char *)::malloc (strdefsize);
        s = buf;
    }

    void reset ()
    {
        strsize = strdefsize;
        strcnt = 0;
        *s = 0;
        bracketdepth = 0;
    }

    void clean () { free (buf); }

    void inc ()
    {
        strcnt++;
        rea ();
    }

    void addchar (const char * c)
    {
        strcnt += 1;
        rea ();
        *s++ = *c;
    }

    void addchars (const char * c)
    {
        size_t i = 0, l = strlen (c);
        strcnt += l;
        rea ();
        do
        {
            *s++ = *c++;
            i++;
        } while (i < l);
    }

    void rea ()
    {
        if (strcnt > strsize)
        {
            buf = (char *)realloc (buf, strsize * 2);
            strsize = strsize * 2;
            s = &buf[strcnt - 1];
        }
    }
};

class VM
{
    void createBaseDicts ();

    void initLexer ();
    void destroyLexer ();

    void loadSoftKernel ();

    void intBinOp (WordPtr One, WordPtr Two, BuiltinTypes type);
    void mixedRealBinOp (WordPtr One, WordPtr Two, BuiltinTypes type);
    void nameBinOp (WordPtr One, WordPtr Two, BuiltinTypes type);

    std::string libPath;

  public:
    WordFactory FactoryW;

    std::vector< WordPtr > DStack;
    std::vector< WordPtr > OStack;

    VM (std::string lib_path) : libPath (lib_path), FactoryW (*this)
    {
        initLexer ();
        createBaseDicts ();
        loadSoftKernel ();
    }
    ~VM () { destroyLexer (); }

    void OPush (WordPtr word) { OStack.push_back (word); }
    WordPtr OPop ()
    {
        WordPtr r = OStack.back ();
        OStack.pop_back ();
        return r;
    }

    LookupResult DictsLookup (std::string token)
    {
        LookupResult ret;
        UpperString (token);
        for (std::vector< WordPtr >::reverse_iterator cell = DStack.rbegin ();
             cell != DStack.rend (); cell++)
        {
            ret = As (WDictionary, (cell))->LookupString (token);
            if (ret.first)
                break;
        }
        return ret;
    }

    void ArrayFromStack ();
    void DictionaryFromStack ();

    void ExecuteIdentifier (std::string token);

    void ExecuteBinOp (WordPtr one, WordPtr two, BuiltinTypes type);

    void Eval (const char * text);

    int LoadFile (std::string name);

    bool CheckStackDepth (unsigned short reqdepth)
    {
        if (OStack.size () < reqdepth)
            return false;
        else
            return true;
    }

    std::vector< void * > yyScanners;
    std::vector< yyVMData > yyVMContexts;
};

#endif
