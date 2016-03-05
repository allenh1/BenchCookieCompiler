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
"bool" return BOOL;
"double" return DOUBLE;
"string" return STRING;
"done!" return DONE;
"please?" return READ; 
"<--" return GETS;
">" return GREAT;
"<" return LESS;
"==" return EQUALS;
"(" return OPAREN;
")" return EPAREN;
"[" return OBRACKET;
"]" return EBRACKET;
"if" return IF;

"+" return PLUS;
"-" return MINUS;
"*" return TIMES;
"/" return DIVIDE;

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
