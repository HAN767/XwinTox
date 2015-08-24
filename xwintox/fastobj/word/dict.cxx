#include "xwforth.h"
#include "xwfword.h"
#include "xwfvm.h"

void WDictionary::Execute (class VM & Vm) { Vm.OPush (Duplicate ()); }

void WDictionary::List ()
{
    std::string open;
    open.append ("<dictionary: " + Name + ": begin>\n");
    printf ("%s", open.c_str ());
    for (DictIter cell = Dictionary.rbegin (); cell != Dictionary.rend ();
         cell++)
    {
        printf (" %s\t: %s\n", cell->key->Describe ().c_str (),
                cell->value->Describe ().c_str ());
    }
    printf ("<dictionary: end>\n");
}

void WDictionary::AddFromWords (WordPtr Word1, WordPtr Word2)
{
    UpperString (Word1->Name);

    for (DictIter cell = Dictionary.rbegin (); cell != Dictionary.rend ();
         cell++)
    {
        if ((cell->key->GetType () != W_NAME) &&
            (cell->key->GetType () != W_STRING))
            continue;
        else
        {
            if (cell->key->Name == Word1->Name)
            {
                cell->value = Word2;
                return;
            }
        }
    }
    {
        DictionaryEntry newEntry;

        newEntry.key = Word1;
        newEntry.value = Word2;
        Dictionary.push_back (newEntry);
    }
}

void WDictionary::AddFromString (std::string Name, WordPtr Word)
{
    DictionaryEntry newEntry;
    newEntry.key = WordPtr (new WName (Name));
    newEntry.value = Word;
    Dictionary.push_back (newEntry);
}

void WDictionary::OpForAll (class VM & Vm, WordPtr Proc)
{
    for (DictIter cell = Dictionary.rbegin (); cell != Dictionary.rend ();
         cell++)
    {
        Vm.OPush (cell->key);
        Vm.OPush (cell->value);
        Proc->Execute (Vm);
    }
}
