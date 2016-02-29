
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

static  void yyunput (int c,char *buf_ptr  );

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
"please?" return READ;
">" return GREAT;
"done!" return DONE;

[^ ^|\t\n]*[^ ^|\t\n]*  {
    yylval.string_val = (char*) calloc(strlen(yytext) + 1, sizeof(char));
    strcpy(yylval.string_val, yytext);
    return WORD;
}

.  {
    return NOTOKEN;
}

