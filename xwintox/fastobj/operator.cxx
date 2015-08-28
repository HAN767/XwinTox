#include "xwforth.h"
#include "xwfword.h"
#include "xwfvm.h"

#define AddBaseOp(name, op)                                                    \
    WordPtr O##op = FactoryW.MakeOperator (op);                                \
    As (WDictionary, &Sys)->AddFromString (name, O##op);

void VM::createBaseDicts ()
{
    WordPtr Sys = FactoryW.MakeDictionary ("SYS");
    WordPtr Global = FactoryW.MakeDictionary ("USER");

    WordPtr Null = FactoryW.MakeInteger (0);
    As (WDictionary, &Sys)->AddFromString ("NULL", Null);

    WordPtr EmptyArray = FactoryW.MakeArray ("-no-name-");
    As (WDictionary, &Sys)->AddFromString ("[]", EmptyArray);

    /* misc */
    AddBaseOp ("TYPE", OP_TYPE);
    AddBaseOp ("XCHECK", OP_XCHECK);
    AddBaseOp ("EXEC", OP_EXEC);
    AddBaseOp ("INCLUDE", OP_INCLUDE);
    AddBaseOp ("PRINT", OP_PRINT);

    /* dictionary operators */
    AddBaseOp (".D", OP_DOTD);
    AddBaseOp (".DICT", OP_DOTDICT);
    AddBaseOp ("DEF", OP_DEF);
    AddBaseOp ("DICTBEGIN", OP_DICTBEGIN);
    AddBaseOp ("DICTEND", OP_DICTEND);
    AddBaseOp ("BEGIN", OP_BEGIN);
    AddBaseOp ("END", OP_END);
    AddBaseOp ("CURRENTDICT", OP_CURRENTDICT);

    /* flow control operators */
    AddBaseOp ("FORALL", OP_FORALL);
    AddBaseOp ("REPEAT", OP_REPEAT);
    AddBaseOp ("IFELSE", OP_IFELSE);
    AddBaseOp ("IF", OP_IF);

    /* manipulating operators */
    AddBaseOp ("==", OP_EQ);
    AddBaseOp ("+", OP_ADD);
    AddBaseOp ("-", OP_SUB);
    AddBaseOp ("*", OP_MUL);
    AddBaseOp ("CVX", OP_CVX);
    AddBaseOp ("GET", OP_GET);
    AddBaseOp ("PUT", OP_PUT);

    /* stack operators */
    AddBaseOp (".S", OP_DOTS);
    AddBaseOp ("CLEAR", OP_CLEAR);
    AddBaseOp ("DUP", OP_DUP);
    AddBaseOp ("POP", OP_POP);
    AddBaseOp ("EXCH", OP_EXCH);

    DStack.push_back (Sys);
    DStack.push_back (Global);
}

void WOperator::Execute (class VM & Vm)
{
    switch (Type)
    {
    /* dictionary */
    case OP_DOTD:
        printf ("<dictionary stack: begin>\n");
        for (std::vector< WordPtr >::reverse_iterator cell =
                 Vm.DStack.rbegin ();
             cell != Vm.DStack.rend (); cell++)
        {
            printf ("  %s\n", (*cell)->Describe ().c_str ());
        }
        printf ("<dictionary stack: end>\n");
        break;

    case OP_DOTDICT:
        As (WDictionary, &Vm.DStack.back ())->List ();
        break;

    case OP_DEF:
    {
        WordPtr val = Vm.OPop (), key = Vm.OPop ();
        if (val->isComposite && !val->isString)
            val->Name.assign (key->Name);
        As (WDictionary, &Vm.DStack.back ())->AddFromWords (key, val);
        break;
    }

    case OP_DICTBEGIN:
    {
        Vm.DStack.push_back (Vm.FactoryW.MakeDictionary ("-no-name-"));
        break;
    }

    case OP_DICTEND:
    {
        Vm.OStack.push_back (Vm.DStack.back ());
        Vm.DStack.pop_back ();
        break;
    }

    case OP_BEGIN:
    {
        Vm.DStack.push_back (Vm.OPop ());
        break;
    }

    case OP_END:
    {
        Vm.OPush (Vm.DStack.back ());
        Vm.DStack.pop_back ();
        break;
    }

    case OP_CURRENTDICT:
    {
        Vm.OPush (Vm.DStack.back ());
        break;
    }

    case OP_GET:
    {
        LookupResult res;
        WordPtr key = Vm.OPop (), dict = Vm.OPop ();
        UpperString (key->Name);
        res = As (WDictionary, &dict)->LookupString (key->Name);
        if (res.first)
            Vm.OPush (res.second);
        break;
    }

    case OP_PUT:
    {
        WordPtr val = Vm.OPop (), key = Vm.OPop ();
        As (WDictionary, &Vm.OStack.back ())->AddFromWords (key, val);
        break;
    }

    /* misc */
    case OP_TYPE:
        Vm.OPush (Vm.OPop ()->GetWordtype ());
        break;

    case OP_XCHECK:
        Vm.OPush (Vm.FactoryW.MakeInteger (Vm.OPop ()->isExecutable ? 1 : 0));
        break;

    case OP_EXEC:
        Vm.OPop ()->Execute (Vm);
        break;

    case OP_INCLUDE:
        Vm.LoadFile (Vm.OPop ()->Name);
        break;

    case OP_PRINT:
        printf ("%s", Vm.OPop ()->Name.c_str ());
        break;

    /* flow control */
    case OP_FORALL:
    {
        WordPtr func = Vm.OPop ();
        WordPtr arr = Vm.OPop ();
        arr->OpForAll (Vm, func);
        break;
    }

    case OP_REPEAT:
    {
        WordPtr func = Vm.OPop ();
        for (short l = Vm.OPop ()->Int.i, c = 0; c < l; c++)
            func->Execute (Vm);
        break;
    }

    case OP_IFELSE:
    {
        WordPtr felse = Vm.OPop (), fif = Vm.OPop (), cond = Vm.OPop ();
        cond->OpTrue () ? fif->Execute (Vm) : felse->Execute (Vm);
        break;
    }

    case OP_IF:
    {
        WordPtr fif = Vm.OPop (), cond = Vm.OPop ();
        if (cond->OpTrue ())
            fif->Execute (Vm);
        break;
    }

    /* mathematical */
    case OP_EQ:
    case OP_ADD:
    case OP_SUB:
    case OP_MUL:
        Vm.ExecuteBinOp (Vm.OPop (), Vm.OPop (), Type);
        break;

    case OP_CVX:
    {
        WordPtr toConv = Vm.OPop ();
        if (toConv->GetType () == W_ARRAY)
        {
            MWordPtr (WArray) asArray = As (WArray, &toConv);
            std::string assembledWords;

            for (std::vector< WordPtr >::iterator cell =
                     asArray->Array.begin ();
                 cell != asArray->Array.end (); cell++)
            {
                printf ("Type: %s\n", (*cell)->GetWordtype ()->Name.c_str ());
                if ((*cell)->GetType () == W_STRING |
                    (*cell)->GetType () == W_NAME)
                {
                    printf ("Append:<%s>\n", (*cell)->Name.c_str ());
                    assembledWords.append ((*cell)->Name);
                    assembledWords.append (" ");
                }
            }
            WordPtr newLambda = Vm.FactoryW.MakeLambda (assembledWords);
            Vm.OPush (newLambda);
        }
        else if (toConv->GetType () == W_LAMBDA)
        {
            Vm.OPush (toConv);
        }
        else if (toConv->GetType () == W_NAME)
        {
            toConv->isExecutable = 1;
            Vm.OPush (toConv);
        }
        break;
    }

    /* stack */
    case OP_DOTS:
        printf ("<operand stack: begin>\n");
        for (std::vector< WordPtr >::reverse_iterator cell =
                 Vm.OStack.rbegin ();
             cell != Vm.OStack.rend (); cell++)
        {
            printf ("  %s\n", (*cell)->Describe ().c_str ());
        }
        printf ("<operand stack: end>\n");
        break;

    case OP_CLEAR:
        Vm.OStack.clear ();
        break;

    case OP_DUP:
        Vm.OPush (Vm.OStack.back ()->Duplicate ());
        break;

    case OP_POP:
        Vm.OPop ();
        break;

    case OP_EXCH:
        WordPtr One = Vm.OPop (), Two = Vm.OPop ();
        Vm.OPush (One);
        Vm.OPush (Two);
        break;
    }
}
