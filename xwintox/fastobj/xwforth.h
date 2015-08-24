#ifndef ___XW_FORTH_H___
#define ___XW_FORTH_H___

#include <algorithm>
#include <cstdio>
#include <map>
#include <memory>
#include <string>
#include <vector>

using namespace std;

#include "xwfplatf.h"

class IWord;
class WOperator;
class WInteger;
class WReal;
class WDictionary;

typedef std::shared_ptr< class IWord > WordPtr;
typedef std::shared_ptr< class WOperator > OpPtr;
typedef std::shared_ptr< class WInteger > IntPtr;
typedef std::shared_ptr< class WReal > RealPtr;
typedef std::shared_ptr< class WDictionary > DictPtr;
typedef std::vector< WordPtr >::iterator StackForwardIter;
typedef std::vector< WordPtr >::reverse_iterator StackIter;
struct DictionaryEntry
{
    WordPtr key;
    WordPtr value;
};
typedef std::vector< struct DictionaryEntry > Dict;
typedef std::vector< struct DictionaryEntry >::reverse_iterator DictIter;
typedef std::pair< bool, WordPtr > LookupResult;

enum BuiltinTypes
{
    /* misc */
    OP_TYPE,
    OP_XCHECK,
    OP_EXEC,
    OP_INCLUDE,
    OP_PRINT,

    /* dictionary operators */
    OP_DEF,
    OP_DOTD,
    OP_DOTDICT,
    OP_DICTBEGIN,
    OP_DICTEND,
    OP_BEGIN,
    OP_END,
    OP_CURRENTDICT,

    /* flow control operators */
    OP_FORALL,
    OP_REPEAT,
    OP_IFELSE,
    OP_IF,

    /* object-dependent/manipulating/etc operators */
    OP_EQ,
    OP_ADD,
    OP_SUB,
    OP_MUL,
    OP_CVX,
    OP_GET,
    OP_PUT,

    /* stack operators */
    OP_DOTS,
    OP_CLEAR,
    OP_DUP,
    OP_POP,
    OP_EXCH,
};

/* This devilish utility template allows us to convert anything. */
template < typename Tout, typename Tin > Tout devilish_cast (Tin in)
{
    union
    {
        Tin in;
        Tout out;
    } result;
    result.in = in;
    return (result.out);
}

#ifndef _NOTRANSFS
inline void UpperString (std::string & data)
{
    std::transform (data.begin (), data.end (), data.begin (), ::toupper);
}
#endif

string iToA (XwfI Number);
string iToA (XwfU Number);
string iToA (double Number);

#endif
