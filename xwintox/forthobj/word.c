#include "ficl.h"


/**************************************************************************
                        w o r d I s I m m e d i a t e
** 
**************************************************************************/
int ficlWordIsImmediate(ficlWord *word)
{
    return ((word != NULL) && (word->flags & FICL_WORD_IMMEDIATE));
}


/**************************************************************************
                        w o r d I s C o m p i l e O n l y
** 
**************************************************************************/
int ficlWordIsCompileOnly(ficlWord *word)
{
    return ((word != NULL) && (word->flags & FICL_WORD_COMPILE_ONLY));
}


/**************************************************************************
                        f i c l W o r d C l a s s i f y
** This public function helps to classify word types for SEE
** and the debugger in tools.c. Given an pointer to a word, it returns
** a member of WOR
**************************************************************************/
ficlWordKind ficlWordClassify(ficlWord *word)
{
    ficlPrimitive code;
    ficlInstruction i;
	ficlWordKind iType;

	if ( (((ficlInstruction)word) > ficlInstructionInvalid)
		&& (((ficlInstruction)word) < ficlInstructionLast) )
		{
		i = (ficlInstruction)word;
		iType = FICL_WORDKIND_INSTRUCTION;
		goto IS_INSTRUCTION;
		}

    code = word->code;

	if ((ficlInstruction)code < ficlInstructionLast)
		{
		i = (ficlInstruction)code;
		iType = FICL_WORDKIND_INSTRUCTION_WORD;
		goto IS_INSTRUCTION;
		}

    return FICL_WORDKIND_PRIMITIVE;

IS_INSTRUCTION:

	switch (i)
	{
		case ficlInstructionConstantParen:
		case ficlInstructionFConstantParen:
			return FICL_WORDKIND_CONSTANT;

		case ficlInstruction2ConstantParen:
		case ficlInstructionF2ConstantParen:
			return FICL_WORDKIND_2CONSTANT;

		case ficlInstructionToLocalParen:
		case ficlInstructionTo2LocalParen:
		case ficlInstructionToFLocalParen:
		case ficlInstructionToF2LocalParen:
			return FICL_WORDKIND_INSTRUCTION_WITH_ARGUMENT;

		case ficlInstructionUserParen:
			return FICL_WORDKIND_USER;

		case ficlInstruction2LiteralParen:
			return FICL_WORDKIND_2LITERAL;

		case ficlInstructionFLiteralParen:
			return FICL_WORDKIND_FLITERAL;

		case ficlInstructionCreateParen:
			return FICL_WORDKIND_CREATE;

		case ficlInstructionCStringLiteralParen:
			return FICL_WORDKIND_CSTRING_LITERAL;

		case ficlInstructionStringLiteralParen:
			return FICL_WORDKIND_STRING_LITERAL;

		case ficlInstructionColonParen:
			return FICL_WORDKIND_COLON;

		case ficlInstructionDoDoes:
	        return FICL_WORDKIND_DOES;

		case ficlInstructionDoParen:
	        return FICL_WORDKIND_DO;

		case ficlInstructionQDoParen:
	        return FICL_WORDKIND_QDO;

		case ficlInstructionVariableParen:
			return FICL_WORDKIND_VARIABLE;

		case ficlInstructionBranchParenWithCheck:
		case ficlInstructionBranchParen:
			return FICL_WORDKIND_BRANCH;
			
		case ficlInstructionBranch0ParenWithCheck:
		case ficlInstructionBranch0Paren:
			return FICL_WORDKIND_BRANCH0;
			
		case ficlInstructionLiteralParen:
			return FICL_WORDKIND_LITERAL;
			
		case ficlInstructionLoopParen:
			return FICL_WORDKIND_LOOP;
			
		case ficlInstructionOfParen:
			return FICL_WORDKIND_OF;
			
		case ficlInstructionPlusLoopParen:
			return FICL_WORDKIND_PLOOP;
			
		default:
			return iType;
	}
}



