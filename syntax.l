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
[\t \n] { /** discard spaces and tabs! **/ }
"print_string" return PRINT_STRING;
"print_int" return PRINT_INT;
"print" { return PRINT_THIS; }
"?" { return WHAAAT; }
"int"  return INT;
"bool" return BOOL;
"double" return DOUBLE;
"string" return STRING;
"do" return DO;
"done!" return DONE;
"please?" return READ; 
"<--" return GETS;
">" return GREAT;
">=" return GREATEQ;
">>" return TOOGREAT;
"<" return LESS;
"<<" return TOOLESS;
"<=" return LESSEQ;
"&&" return LAND;
"&" return AND;
"||" return LOR;
"|" return OR;
"^^" return LXOR;
"^" return XOR;
"!" return NOT;
"~" return TWIDLE;
"=" return EQUALS;
"!=" return NOTEQUALS;
"(" return OPAREN;
")" return EPAREN;
"[" return OBRACKET;
"]" return EBRACKET;
"if" return IF;
"else" return ELSE;
"then" return THEN;
"endif" return ENDIF;
"for" return FOR;
"endfor" return ENDFOR;

"+" return PLUS;
"-" return MINUS;
"*" return TIMES;
"/" return DIVIDE;

\"(\\.|[^"])*\" { /*"*/
	char * p;
	for (p = yytext + 1; *p && *p != '\"'; ++p); /*"*/
	yylval.string_val = (char*) calloc(p - yytext + 1, sizeof(char));
	strncpy(yylval.string_val, yytext + 1, p - (yytext + 1));
	return STRING_LITERALLY;
}

[^ ^|\t\n]*[^ ^|\t\n]*  {
    yylval.string_val = (char*) calloc(strlen(yytext) + 1, sizeof(char));
    strcpy(yylval.string_val, yytext);
    return WORD;
}

