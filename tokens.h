/**
 * This header contains the tokens for the language.
 */
#ifndef __TOKENS_H__
#define __TOKENS_H__
#define OBRACKET   0
#define CBRACKET   1
#define OBRACE     2
#define CBRAKE     3
#define OPAREN     4
#define CPAREN     5
#define PLUS       6
#define MINUS      7
#define STAR       8
#define SEMICOLON  9
#define UNDERSCORE 10
#define BANG       11
#define FUNC       12
#define COMMA      13
#define FSLASH     14
#define BSLASH     15
#define AND        16
#define OR         17
#define AT         18
#define QUOTE      19
#define APOSTROPHE 20
#define DIGIT      21
#define ALPHA      22
#define INTEGER    23
#define FLOAT      24
#define NOTOKEN    25

#include <string.h>
#include <stdlib.h>
#include <stdio.h>

/* useful macros */
#define IS_ALPHA(a) (a >= 'a' && a <= 'z') || (a >= 'A' && a <= 'Z')
#define IS_DIGIT(a) a >= '0' && a <= '9'

/* scanner functions */
int matches_obracket (const char * str);
int matches_cbracket (const char * str);
int matches_obrace (const char * str);
int matches_cbrace (const char * str);
int matches_oparen (const char * str);
int matches_cparen (const char * str);
int matches_plus (const char * str);
int matches_minus (const char * str);
int matches_star (const char * str);
int matches_semicolon (const char * str);
int matches_underscore (const char * str);
int matches_bang (const char * str);
int matches_func (const char * str);
int matches_comma (const char * str);
int matches_fslash (const char * str);
int matches_bslash (const char * str);
int matches_and (const char * str);
int matches_or (const char * str);
int matches_at (const char * str);
int matches_quote (const char * str);
int matches_apostrophe (const char * str);
int matches_alpha (const char * str);
int matches_digit (const char * str);
#endif
