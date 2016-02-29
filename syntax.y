
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

%token	<string_val> WORD

%token PRINT_STRING INT READ GREAT STRING DONE
%token PRINT_INT WHAAAT PRINT_THIS NOTOKEN

%union	{
	char * string_val;
}

%{
//#define yylex yylex
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
	| DONE {
	  return 0;
	}
	;

line:
	read_line
	| print_line
	|
	;

read_line:
        INT READ GREAT WORD {
	  // std::cout<<"Reading great words of int"<<std::endl;
	  Command::cmd.addReadInt($4);
	}
        | STRING READ GREAT WORD {
	  // std::cout<<"Reading great words of string"<<std::endl;
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
       // std::cerr<<"Reading some number stuff to say"<<std::endl;
       Command::cmd.addPrintInt($2);
     }
     ;

print_string:
    PRINT_STRING WORD WHAAAT {
      // std::cerr<<"Reading some string stuff to say"<<std::endl;
      Command::cmd.addPrintString($2);
    }
    ;

print_literal:
    PRINT_THIS WORD WHAAAT {
      // std::cerr<<"You want me to say WHAAAT?"<<std::endl;
      Command::cmd.addPrintLiteral($2);
    }
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
