%{

#include <string.h>
#include <iostream>
#include "y.tab.h"

static  void yyunput (int c, char * buf_ptr);

void myunputc(int c) {
    unput(c);
}

%}

%option yylineno

%x BLK_COMMENT
%x LINE_COMMENT
%%
[\t ]+ { /** discard spaces and tabs! **/ }
[\n] { ++yylineno; }

<INITIAL>"@;" BEGIN(BLK_COMMENT);
<BLK_COMMENT>"@;" BEGIN(INITIAL);
<BLK_COMMENT>[.\n]+  { /** discard anything in a comment block **/ }

<INITIAL>"@" BEGIN(LINE_COMMENT);
<INITIAL>";" BEGIN(LINE_COMMENT);
<LINE_COMMENT>[\n] BEGIN(INITIAL);
<LINE_COMMENT>[.]+ { /** disregard anything in a line comment **/ }

"print_string" return PRINT_STRING;
"print_int" return PRINT_INT;
"print" { return PRINT_THIS; }
"?" { return WHAAAT; }
":" { return COLON; }
"int"  return INT;
"line" return LINE;
"char" return CHAR;
"deref" return DEREF;
"func" return FUNC;
"end" return END;
"int_ptr" return INT_PTR;
"char_ptr" return CHAR_PTR;
"bool" return BOOL;
"double" return DOUBLE;
"declare" return DECLARE;
"c_function" return C_FUNC;
"string" return STRING;
"do" return DO;
"done!" return DONE;
"please?" return READ; 
"<--" return GETS;
"-->" return POINT_TO;
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

[^ ^|\t\n\[\]\(\)\+\-\*\/]*[^ ^|\t\n\[\]\(\)\+\-\*\/]*  {
    yylval.string_val = (char*) calloc(strlen(yytext) + 1, sizeof(char));
    strcpy(yylval.string_val, yytext);
    return WORD;
}

