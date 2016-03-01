
/*
 *
 * CS-252 Spring 2016
 * shell.l: lexical analyzer for shell
 * You have to extend it.
 *
 */

%{

#include <string.h>
#include <iostream>
#include "y.tab.h"

static  void yyunput (int c, char * buf_ptr);

void myunputc(int c) {
    unput(c);
}

%}

%%
[ \t] { /** discard spaces and tabs! **/ }
"print_string" return PRINT_STRING;
"print_int" return PRINT_INT;
"print" { return PRINT_THIS; }
"?" { return WHAAAT; }
"int"  return INT;
"string" return STRING;
"done!" return DONE;
"please?" return READ;
">" return GREAT;
"}" return EBRACE;
"{" return OBRACE;
"func" return FUNC;
"(" return OPAREN;
")" return EPAREN;
"[" return OBRACKET;
"]" return EBRACKET;

\"(\\.|[^"])*\" { /*"*/
	char * p;
	for (p = yytext + 1; *p && *p != '\"'; ++p); /*"*/
	yylval.string_val = (char*) calloc(p - yytext, sizeof(char));
	strncpy(yylval.string_val, yytext + 1, p - (yytext + 1));
	return STRING_LITERALLY;
}

[^ ^|\t\n]*[^ ^|\t\n]*  {
    yylval.string_val = (char*) calloc(strlen(yytext) + 1, sizeof(char));
    strcpy(yylval.string_val, yytext);
    return WORD;
}

.  {
    return NOTOKEN;
}

