%{#include <memory>%}
%token <string_val> WORD INTEGER DOUBLE
%token <string_val> STRING_LITERALLY

%token PRINT_STRING INT READ GREAT STRING DONE EPAREN
%token PRINT_INT WHAAAT PRINT_THIS NOTOKEN NEW_LINE BOOL ELSE
%token OBRACKET EBRACKET IF COLON OPAREN EQUALS LESS GETS

%token PLUS MINUS TIMES DIVIDE AND GREATEQ LAND LOR LESSEQ
%token LXOR XOR NOTEQUALS OR TOOGREAT TOOLESS THEN ENDIF
%token ENDFOR ENDIF DO

%nonassoc IF ELSE THEN ENDIF FOR DO ENDFOR ELIF

%left PLUS MINUS LOR LESSEQ LXOR XOR NOTEQUALS
%left TIMES DIVIDE TOOGREAT TOOLESS
%left OR GREATEQ 
%left OPAREN EPAREN

%right GETS NOT TWIDLE

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
	| if_else_block
	| loop
	| declare
	| assignment
        | DONE { return 0; }
        ;

read_line:
    INT READ GREAT WORD { Command::cmd.addReadInt($4); }
    | STRING READ GREAT WORD { Command::cmd.addReadString($4); }
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

declare:
    INT WORD WHAAAT { Command::cmd.declInt($2); }
//    | DOUBLE WORD WHAAAT { Command::cmd.declDouble($2); }
//    | STRING WORD WHAAAT { Command::cmd.declString($2); }
//    | BOOL WORD WHAAAT { Command::cmd.declBool($2); }
    ;

assignment:
    WORD GETS expr { Command::cmd.addIntAssignment($1); }
    ;

if_else_block:
    IF WORD THEN { Command::cmd.startIfBlock($2); }
//    | ELIF WORD THEN { Command::cmd.startElIfBlock($2); }
    | ENDIF { Command::cmd.endIfBlock(); }
    ;

expr:
    exp WHAAAT { Command::cmd.markEndOfExpression(); }
    | exp expr
    ;

exp:
    WORD { Command::cmd.addToExpressionStack($1); }
    | exp LOR exp { Command::cmd.addToExpressionStack(strdup("||")); }
    | exp LXOR exp { Command::cmd.addToExpressionStack(strdup("^^")); }
    | exp LAND exp { Command::cmd.addToExpressionStack(strdup("&&")); }
    | exp OR exp { Command::cmd.addToExpressionStack(strdup("|")); }
    | exp XOR exp { Command::cmd.addToExpressionStack(strdup("^")); }
    | exp AND exp { Command::cmd.addToExpressionStack(strdup("&")); }
    | exp NOTEQUALS exp { Command::cmd.addToExpressionStack(strdup("!=")); }
    | exp EQUALS exp { Command::cmd.addToExpressionStack(strdup("==")); }
    | exp GREAT exp { Command::cmd.addToExpressionStack(strdup(">")); }
    | exp GREATEQ exp { Command::cmd.addToExpressionStack(strdup(">=")); }
    | exp LESS exp { Command::cmd.addToExpressionStack(strdup("<")); }
    | exp LESSEQ exp { Command::cmd.addToExpressionStack(strdup("<=")); }
    | exp TOOGREAT exp { Command::cmd.addToExpressionStack(strdup(">>")); }
    | exp TOOLESS exp { Command::cmd.addToExpressionStack(strdup("<<")); }
    | exp PLUS exp { Command::cmd.addToExpressionStack(strdup("+")); }
    | exp MINUS exp { Command::cmd.addToExpressionStack(strdup("-")); }
    | exp TIMES exp { Command::cmd.addToExpressionStack(strdup("*")); }
    | exp DIVIDE exp { Command::cmd.addToExpressionStack(strdup("/")); }
    | NOT exp { Command::cmd.addToExpressionStack(strdup("!")); }
    | TWIDLE exp { Command::cmd.addToExpressionStack(strdup("~")); }
    | PLUS exp { Command::cmd.addToExpressionStack(strdup("u+")); }
    | MINUS exp { Command::cmd.addToExpressionStack(strdup("u-")); }
    | OPAREN exp EPAREN { }
    ;

bounds:
    '[' body ']'
    | line
    | loop
    | WHAAAT
    ;

body:
    body body
    | line
    | loop
    |
    ;

loop:
    FOR WORD DO { Command::cmd.markForLoop($2); }
    | ENDFOR { Command::cmd.markEndFor(); }
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
