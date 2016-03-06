%{#include <memory>%}
%token <string_val> WORD INTEGER DOUBLE
%token <string_val> STRING_LITERALLY

%token PRINT_STRING INT READ GREAT STRING DONE EPAREN
%token PRINT_INT WHAAAT PRINT_THIS NOTOKEN NEW_LINE BOOL
%token OBRACKET EBRACKET IF COLON OPAREN EQUALS LESS GETS

%token PLUS MINUS TIMES DIVIDE

%left PLUS MINUS
%left TIMES DIVIDE
%left NEGATE

%union	{
	char * string_val;
}

%{
#include <stdio.h>
#include <iostream>
#include <cmath>
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
goal: lines

lines:
    lines line
	| line
	;

line:
	read_line
	| print_line
	| assignment
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

//declare:
//    INT WORD WHAAAT
//   | DBL WORD WHAAAT Command::cmd.declDouble($2);
//  | STRING WORD WHAAAT Command::cmd.declString($2);
//  | BOOL WORD WHAAAT Command::cmd.declBool($2);
//    ;

assignment:
    WORD GETS expr { std::cerr<<"Called gets"<<std::endl; Command::cmd.markEndOfExpression(); Command::cmd.addIntAssignment($1); }
    ;

number:
    WORD {
      Command::cmd.addToExpressionStack($1);
    } | INTEGER {
      Command::cmd.addToExpressionStack($1);
    } | DOUBLE {
      Command::cmd.addToExpressionStack($1);
    }
    ;

expr:
    expr exp
    | exp
    ;

exp:
    | number
    | exp PLUS exp { Command::cmd.addToExpressionStack(strdup("+")); }
    | exp MINUS exp { Command::cmd.addToExpressionStack(strdup("-")); }
    | exp TIMES exp { Command::cmd.addToExpressionStack(strdup("*")); }
    | exp DIVIDE exp { Command::cmd.addToExpressionStack(strdup("/")); }
    | OPAREN exp EPAREN { }
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
