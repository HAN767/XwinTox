/* This represents any whitespace character of any number of occurences. */
space       [ \t\n\r\f]+
digit       [0-9]
alpha       [a-fA-F]
/* This represents the component of a hexadecimal number occuring after 0x */
hextail     ({digit}|{alpha}){1,8}
/* This assembles the above with a 0x, allowing us to match hex numbers. */
hex         0[xX]{hextail}
/* This, any decimal number, sans decimal point. */
dec         [0123456789]+

/* yywrap() is called by Lex when it runs out of input to lex. As we just supply
 * to the lexer those buffers we need processed, as-and-when we need to, we
 * don't need it. */
%option noyywrap
/* This option tells Lex to produce a re-entrant lexer. This turns off the use
 * of global variables, which aren't suitable for a threaded application such
 * as XwinFast. Further, it lets us specify custom data to the lexer (see the
 * next line) without using a global variable. */
%option reentrant
/* Enabling the reentrant option lets us specify custom data for the lexer.
 * XwinFast uses this to let the lexer access the VM. */
%option extra-type="class VM *"

/* The %x denotes an 'eXclusive' start state. A start state allows us to define
 * special rules that should be evaluated by the lexer after we've entered such
 * a state. It gives us the ability to maintain different grammar for different
 * situations. The exclusive simply means that the default grammar is not used.
 * XwinFast uses this to help it parse a few special types. */
%x STRING
%x LAMBDA
%x NAME
%x WORD
%x LCOMMENT

%{
#include "xwforth.h"
#include "xwfword.h"
#include "xwfvm.h"

/* Just for convenience. */
#define Vm yyextra
#define CTX yyextra->yyVMContexts.back()

/* Let us first handle the case of missing <unistd.h>. */

#ifndef HAVE_UNISTD_H
#define YY_NO_UNISTD_H
#define isatty(x) 0
#endif
%}

%%

 /* If any whitespace is matched, we will do nothing with it. */
{space}         { }

 /* If we encounter a %, then let's enter the 'LCOMMENT' eXclusive start 
  * state. */
\% { BEGIN LCOMMENT; }

 /* If we encounter a newline, then we'll return to our default start state. */
<LCOMMENT>\n    { BEGIN 0; }

 /* If we encounter anything else, we don't do anything. */
<LCOMMENT>.     { }

 /* If a word begins with a forward-slash (/,) then it's surely a name */
\/              {
                  /* So let's enter the 'NAME' eXclusive start state. */
                  BEGIN NAME;
                  /* This sets up the yyVMContext, where we store things we're
                   * in the process of assembling. */
                  CTX.init ();
                }

 /* Now we're in the NAME start state. This rule matches spaces, but only in the
  * NAME start state. Once we reach a space we can push our formed word. */
<NAME>{space}   {
                  /* Get the yyVMContext ready for a new word. */
                  CTX.reset ();
                  //printf("Found a name <%s>\n", CTX.buf);
                  Vm->OPush(Vm->FactoryW.MakeName(CTX.buf));
                  /* Clean up the yyVMContext now we're done with it. */
                  CTX.clean ();
                  /* Return to the default start state, so we're ready to lex as
                   * normal again. */
                  BEGIN 0;
                }

 /* But any other character is just added to the word we're assembling. This is
  * below the rule for a space as Lex tries to match the first rule it
  * encounters. */
<NAME>.         { CTX.addchar (yytext); }

 /* An opening curved bracket ({), followed by a space, marks the beginning of a
  * lambda function. */
\{{space}       { BEGIN LAMBDA; CTX.init (); }

 /* If it contains an escaped character, add that character to the lambda being
 *  assembled. This lets us preserve strings. */
<LAMBDA>\\n     { CTX.addchars("\\n"); }
<LAMBDA>\\t     { CTX.inc (); *CTX.s++ = '\t'; }
<LAMBDA>\\\"    { CTX.inc (); *CTX.s++ = '\"'; }

<LAMBDA>\{      { CTX.bracketdepth++; CTX.addchar (yytext); }

 /* Finally, a a closing curved bracket (}) followed by a space marks the end of
  * a lambda function. */
<LAMBDA>\}{space}      {
                  if (CTX.bracketdepth > 0)
                  {
                    CTX.bracketdepth--;
                    CTX.addchar (yytext);
                    CTX.addchar (" ");
                  }
                  else
                  {
                    CTX.reset ();
                    //printf("Found a lambda: %s\n", CTX.buf);
                    Vm->OPush(Vm->FactoryW.MakeLambda(CTX.buf));
                    CTX.clean ();
                    BEGIN 0;
                  }
                }

 /* As above, anything else we haven't explicitly handled is just added into the
  * lambda body. */
<LAMBDA>.       { CTX.addchar (yytext); }
<LAMBDA>{space} { CTX.addchar (yytext); }

 /* Now we handle strings, enclosed in quotation marks. */
\"              { BEGIN STRING; CTX.init (); }

 /* Escaped characters are de-escaped before we add them here. */
<STRING>\\n     { CTX.inc (); *CTX.s++ = '\n'; }
<STRING>\\t     { CTX.inc (); *CTX.s++ = '\t'; }
 /* Someone may wish to embed a quotation mark within their string. So, if it's
  * escaped, we will do so. Note that this rule is above the terminating-a-string
  * rule; this means that Lex tries to match it before it tries to match a single
  * unescaped quotation mark */
<STRING>\\\"    { CTX.inc (); *CTX.s++ = '\"'; }

 /* But if it is a single quotation mark, unescaped, followed by a space, then
  * it's time to assemble the string and push it. */
<STRING>\"{space} { 
                  CTX.reset ();
                  //printf("Found a string: %s\n", CTX.buf);
                  Vm->OPush(Vm->FactoryW.MakeString(CTX.buf));
                  CTX.clean ();
                  BEGIN 0;
                }
<STRING>.       { CTX.addchar (yytext); }

 /* Handling numbers is simple; we just push a freshly-made number word, created
  * via strtol, the modern version of atoi that lets us specify which base the
  * number to convert is in. */

{hex}{space}    { Vm->OPush(Vm->FactoryW.MakeInteger(strtol(yytext, 0, 16))); }

{dec}{space}    { Vm->OPush(Vm->FactoryW.MakeInteger(strtol(yytext, 0, 10))); }

 /* These mark the beginning and the end of a dictionary entry. It's as simple
  * as adding a mark to the stack, continuing to lex as normal, and then - after
  * we reach the >> (dictionary ending) point - associating each pair of words
  * after the mark with the dictionary. */

\<\<{space}     { Vm->OPush(Vm->FactoryW.MakeMarker()); }

\>\>{space}     { Vm->DictionaryFromStack(); }

 /* We do likewise for arrays. */

\({space}     { Vm->OPush(Vm->FactoryW.MakeMarker()); }

\){space}     { Vm->ArrayFromStack(); }

 /* Anything that we have yet to match is interpreted as being as a word. */
.               { BEGIN WORD; CTX.init (); *CTX.s++ = *yytext; }

<WORD>{space}   {
                  CTX.reset ();
                  std::string id(CTX.buf);
                  CTX.clean ();
                  //printf("Found a word: %s\n", id.c_str());
                  /* So we can just execute it. */
                  Vm->ExecuteIdentifier(id);
                  BEGIN 0;
                }
<WORD>.         { CTX.addchar (yytext); }

%%

void VM::initLexer () { }

void VM::destroyLexer () { }

void VM::Eval (const char * text)
{
    /* Lex asks us to scan our string into a buffer that is is willing to work
     * with. */
    YY_BUFFER_STATE yybuffer;
    /* Create a new yyVMData for the newly created context to work with. */
	yyVMData newCtx;

    /* Push back the new yyVMData to the contexts vector. */
	yyVMContexts.push_back(newCtx);
	/* And push back a new space for a yyScanner context in the scanner context
     * vector. */
	yyScanners.push_back(0);

    yylex_init_extra (this, &yyScanners.back());
    yybuffer = yy_scan_string (text, yyScanners.back());
	try
	{
    yylex (yyScanners.back());
	}
	catch(int)
	{ }
    yy_delete_buffer (yybuffer, yyScanners.back());
    yylex_destroy (yyScanners.back());

    /* Now, pop the old data context and scanner context. */
	yyVMContexts.pop_back();
	yyScanners.pop_back();
}
