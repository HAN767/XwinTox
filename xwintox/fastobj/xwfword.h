#ifndef __XWINWORD_H
#define __XWINWORD_H

#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <list>
#include <string>
#include <vector>

#include "xwfplatf.h"
#include "xwforth.h"

enum WordTypes
{
    W_NONE,
    W_NAME,
    W_OPERATOR,
    W_MARKER,
    W_INT,
    W_REAL,
    W_LAMBDA,
    W_STRING,
    W_ARRAY,
    W_DICT,
};

class IWord
{
  protected:
    /* required */
    WordTypes type;

  public:
    std::string Name; /* since it's common to a few */
    union ic
    {
        double d;
        XwfI i;
    };
    ic Int;
    char isExecutable, isComposite, isString;
    WordPtr * selfref;

    IWord (WordTypes typ = W_NONE)
        : type (typ), Name ("-no-string-value"), isExecutable (0),
          isComposite (0)
    {
    }

    WordTypes GetType () { return type; }

    virtual void Execute (class VM & Vm) = 0;
    virtual std::string Describe () = 0;
    virtual WordPtr Duplicate () = 0;
    virtual WordPtr GetWordtype () = 0; /* returns a WName */

    virtual bool OpTrue () { return true; }
    virtual void OpForAll (class VM & Vm, WordPtr Proc)
    {
        std::printf ("Error: TypeError: object is not composite");
    }
};

class WName : public IWord
{
  public:
    WName (std::string nam) : IWord (W_NAME)
    {
        Name = nam;
        isComposite = 1;
        isString = 1;
    }

    void Execute (class VM & Vm);
    std::string Describe () { return ("<name: " + Name + ">"); }
    WordPtr Duplicate () { return WordPtr (new WName (*this)); }
    WordPtr GetWordtype () { return WordPtr (new WName ("nametype")); }
};

class WOperator : public IWord
{
  public:
    BuiltinTypes Type;

    WOperator (BuiltinTypes typ) : IWord (W_OPERATOR), Type (typ) {}

    void Execute (class VM & Vm);
    std::string Describe () { return ("<operator>"); }

    WordPtr Duplicate () { return WordPtr (new WOperator (*this)); }
    WordPtr GetWordtype () { return WordPtr (new WName ("operatortype")); }
};

class WMarker : public IWord
{
  public:
    WMarker () : IWord (W_MARKER) {}

    void Execute (class VM & Vm) {}
    std::string Describe () { return ("<marker>"); }
    WordPtr Duplicate () { return WordPtr (new WMarker (*this)); }
    WordPtr GetWordtype () { return WordPtr (new WName ("markertype")); }
};

class WInteger : public IWord
{
  public:
    WInteger (XwfI num) : IWord (W_INT) { Int.i = num; }

    void Execute (class VM & Vm);
    std::string Describe () { return ("<integer>"); }
    WordPtr Duplicate () { return WordPtr (new WInteger (*this)); }
    WordPtr GetWordtype () { return WordPtr (new WName ("integertype")); }

    bool OpTrue ()
    {
        if (Int.i)
            return true;
        else
            return false;
    }
};

class WReal : public IWord
{
  public:
    WReal (double num) : IWord (W_REAL) { Int.d = num; }

    void Execute (class VM & Vm);
    std::string Describe () { return ("<real>"); }
    WordPtr Duplicate () { return WordPtr (new WReal (*this)); }
    WordPtr GetWordtype () { return WordPtr (new WName ("realrtype")); }

    bool OpTrue ()
    {
        if (Int.d != 0)
            return true;
        else
            return false;
    }
};

class WLambda : public IWord
{
    std::string content;

  public:
    WLambda (std::string cont) : IWord (W_LAMBDA), content (cont)
    {
        isExecutable = 1;
        isComposite = 1;
    }

    void Execute (class VM & Vm);
    std::string Describe () { return ("<lambda>"); } //+ content + ">"); }
    WordPtr Duplicate () { return WordPtr (new WLambda (*this)); }
    WordPtr GetWordtype () { return WordPtr (new WName ("lambdatype")); }

    void OpForAll (class VM & Vm, WordPtr Proc) {}
};

class WString : public IWord
{
  public:
    WString (std::string nam) : IWord (W_STRING)
    {
        Name = nam;
        isComposite = 1;
        isString = 1;
    }

    void Execute (class VM & Vm);
    std::string Describe () { return ("<string: " + Name + ">"); }
    WordPtr Duplicate () { return WordPtr (new WString (*this)); }
    WordPtr GetWordtype () { return WordPtr (new WName ("stringtype")); }

    void OpForAll (class VM & Vm, WordPtr Proc) {}
};

class WDictionary : public IWord
{
  public:
    Dict Dictionary;

    void AddFromWords (WordPtr Word1, WordPtr Word2);
    void AddFromString (std::string Name, WordPtr Word);
    void List ();

    LookupResult LookupString (std::string token)
    {
        LookupResult ret;
        ret.first = false;
        // if(token == nullptr) return ret;
        for (DictIter cell = Dictionary.rbegin (); cell != Dictionary.rend ();
             cell++)
        {
            if ((cell->key->GetType () != W_NAME) &&
                (cell->key->GetType () != W_STRING))
                continue;
            else
            {
                if (cell->key->Name == token)
                {
                    ret.first = true;
                    ret.second = (*cell).value;
                    return ret;
                }
            }
        }
        return ret;
    }

    WDictionary (std::string nam) : IWord (W_DICT)
    {
        Name = nam;
        isComposite = 1;
    }

    void Execute (class VM & Vm);
    std::string Describe () { return ("<dictionary: " + Name + ">"); }
    WordPtr Duplicate () { return WordPtr (new WDictionary (*this)); }
    WordPtr GetWordtype () { return WordPtr (new WName ("dictionarytype")); }

    void OpForAll (class VM & Vm, WordPtr Proc);
};

class WArray : public IWord
{
  public:
    std::vector< WordPtr > Array;

    WArray (std::string nam) : IWord (W_ARRAY)
    {
        isComposite = 1;
        Name = nam;
    }

    void Execute (class VM & Vm);
    std::string Describe ()
    {
        return ("<array: " + iToA (Array.size ()) + ">");
    }
    WordPtr Duplicate () { return WordPtr (new WArray (*this)); }
    WordPtr GetWordtype () { return WordPtr (new WName ("arraytype")); }

    void AddWord (WordPtr word) { Array.push_back (word); }

    void OpForAll (class VM & Vm, WordPtr Proc);
};

class WordFactory
{
    class VM & vm;

  public:
    WordFactory (class VM & Vm) : vm (Vm) {}

    WordPtr MakeName (std::string name) { return WordPtr (new WName (name)); }

    WordPtr MakeOperator (BuiltinTypes typ)
    {
        return WordPtr (new WOperator (typ));
    }

    WordPtr MakeMarker () { return WordPtr (new WMarker ()); }

    WordPtr MakeInteger (XwfI val) { return WordPtr (new WInteger (val)); }

    WordPtr MakeReal (double val) { return WordPtr (new WReal (val)); }

    WordPtr MakeLambda (std::string content)
    {
        return WordPtr (new WLambda (content));
    }

    WordPtr MakeString (std::string text)
    {
        return WordPtr (new WString (text));
    }

    WordPtr MakeDictionary (std::string name)
    {
        return WordPtr (new WDictionary (name));
    }

    WordPtr MakeArray (std::string name) { return WordPtr (new WArray (name)); }
};

#if (WATCOM == 1)
#pragma xr
#define MWordPtr(typ) typ *
#define As(typ, x) dynamic_cast< typ * > (((x)->get ()))
#else
#define MWordPtr(typ) std::shared_ptr< typ >
#define As(typ, x) std::dynamic_pointer_cast< typ > (*x)
#endif
#endif
