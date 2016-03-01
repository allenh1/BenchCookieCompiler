
/*
 * CS-252
 * shell.y: parser for shell
 *
 * This parser compiles the following grammar:
 *
 *	cmd [arg]* [> filename] [< filename]
 *
 * you must extend it to understand the complete shell grammar
 *
 */
%{#include <memory>%}

%token <string_val> WORD
%token <string_val> STRING_LITERALLY

%token PRINT_STRING INT READ GREAT STRING DONE
%token PRINT_INT WHAAAT PRINT_THIS NOTOKEN NEW_LINE
%token EBRACE OBRACE FUNC OPAREN EPAREN OBRACKET EBRACKET

%union	{
	char * string_val;
}

%{
#include <stdio.h>
#include <iostream>
#include "command.h"
void yyerror(const char * s);
 int yylex();
%}

%%
/**
 * Lex: 
 * <reg. exp> <whitespace> <action>
 *
 * <reg. exp>: beginning of line up to first non-escaped white space.
 *
 * <action>: A single C statement. Multiple statements are wrapped in braces.
 */
goal: lines;

lines:
    lines line
	| line
	;

line:
	read_line
	| print_line
	| function
    | DONE {
		return 0;
	}
	;

read_line:
    INT READ GREAT WORD {
		Command::cmd.addReadInt($4);
	}
    | STRING READ GREAT WORD {
		Command::cmd.addReadString($4);
	}
    ;

print_line:
    print_int
    | print_string
    | print_literal
    ;

print_int:
    PRINT_INT WORD WHAAAT {
       Command::cmd.addPrintInt($2);
    }
    ;

print_string:
    PRINT_STRING WORD WHAAAT {
      Command::cmd.addPrintString($2);
    }
    ;

print_literal:
    PRINT_THIS STRING_LITERALLY WHAAAT {
      Command::cmd.addPrintLiteral($2);
    }
    ;

function:
    FUNC "main" OPAREN EPAREN OBRACE lines {
        std::cout<<"Matched main"<<std::endl;
    }
    | FUNC WORD OPAREN EPAREN OBRACE lines EBRACE
	;
%%
void yyerror(const char * s)
{
	fprintf(stderr,"%s", s);
}

#if 0
main()
{
	yyparse();
}
#endif
