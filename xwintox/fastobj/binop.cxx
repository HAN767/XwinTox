#include "xwforth.h"
#include "xwforth.h"
#include "xwfvm.h"

#define XInvolved(W) (one->GetType () == W || two->GetType () == W)

void VM::ExecuteBinOp (WordPtr one, WordPtr two, BuiltinTypes type)
{
    bool IntegerInvolved = false, RealInvolved = false, NameInvolved = false,
         StringInvolved = false;

    NameInvolved = XInvolved (W_NAME);
    StringInvolved = XInvolved (W_NAME);

    if (one->GetType () == W_INT || two->GetType () == W_INT)
        IntegerInvolved = true;

    if (one->GetType () == W_REAL || two->GetType () == W_REAL)
        RealInvolved = true;

    if (RealInvolved && IntegerInvolved)
        mixedRealBinOp (one, two, type);
    else if (IntegerInvolved)
        intBinOp (one, two, type);
    else if (NameInvolved || (NameInvolved && StringInvolved))
        nameBinOp (one, two, type);
    /*else if (StringInvolved)
        StringBinOp (one, two, type);*/
}

void VM::nameBinOp (WordPtr one, WordPtr two, BuiltinTypes type)
{
    switch (type)
    {
    case OP_EQ:
        UpperString (one->Name);
        UpperString (two->Name);
        OPush (FactoryW.MakeInteger ((one->Name == two->Name) ? 1 : 0));
        break;
    default:
        ;
    }
}

void VM::intBinOp (WordPtr one, WordPtr two, BuiltinTypes type)
{
    switch (type)
    {
    case OP_ADD:
        OPush (FactoryW.MakeInteger (one->Int.i + two->Int.i));
        break;
    case OP_MUL:
        OPush (FactoryW.MakeInteger (one->Int.i * two->Int.i));
        break;
    case OP_SUB:
        OPush (FactoryW.MakeInteger (one->Int.i - two->Int.i));
        break;
    default:
        ;
    }
}

void VM::mixedRealBinOp (WordPtr tOne, WordPtr tTwo, BuiltinTypes type)
{
    WordPtr one;
    WordPtr two;
    bool inverted;

    if (tOne->GetType () == W_INT)
    {
        one = tOne;
        two = tTwo;
    }
    else if (tTwo->GetType () == W_REAL)
    {
        one = tTwo;
        two = tOne;
        inverted = true;
    }
    switch (type)
    {
    case OP_ADD:
        OPush (FactoryW.MakeReal (((double)one->Int.i) + (double)two->Int.d));
        break;
    case OP_MUL:
        OPush (FactoryW.MakeReal (((double)one->Int.i) * (double)two->Int.d));
        break;
    case OP_SUB:
        if (!inverted)
        {
            OPush (
                FactoryW.MakeReal (((double)one->Int.i) - (double)two->Int.d));
            break;
        }
        else
        {
            OPush (
                FactoryW.MakeReal (((double)two->Int.d) - (double)one->Int.i));
            break;
        }
    default:
        ;
    }
}
