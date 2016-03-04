%{#include <memory>%}

%token <string_val> WORD
%token <string_val> STRING_LITERALLY

%token PRINT_STRING INT READ GREAT STRING DONE EPAREN
%token PRINT_INT WHAAAT PRINT_THIS NOTOKEN NEW_LINE
%token OBRACKET EBRACKET IF COLON OPAREN EQUALS LESS

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

exp:
    WORD GREAT WORD {
      
    }
    | WORD LESS WORD {

    }
    | WORD EQUALS WORD {

    }
    ;
 
if_block:
    IF exp OBRACKET lines EBRACKET
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
