/*
 * This header contains the tokens for the language.
 */
#ifndef __TOKENS_H__
#define __TOKENS_H__
/* C lib includes */
#include <assert.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>

/* token constants */
#define OBRACKET   0
#define CBRACKET   1
#define OBRACE     2
#define CBRACE     3
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
#define AMPERSAND  16
#define PIPE       17
#define AT         18
#define QUOTE      19
#define APOSTROPHE 20
#define DIGIT      21
#define ALPHA      22
#define INTEGER    23
#define FLOAT      24
#define NOTOKEN    25
#define CAPTURE    26
#define MAYBE      27
#define INT        28
#define BOOL       29
#define CHAR       30
#define DOUBLE     31
#define LONG       32
#define UNSIGNED   33
#define QUESTION   34
#define AND        35
#define OR         36
#define JUST       37

/* token containers */
struct token {
		char * image;
		int tok;
};

struct token_node {
		struct token_node * next;
		struct token tok;
};

struct token_list {
		struct token_node * head;
};

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
int matches_ampersand (const char * str);
int matches_pipe (const char * str);
int matches_at (const char * str);
int matches_quote (const char * str);
int matches_apostrophe (const char * str);
int matches_alpha (const char * str);
int matches_digit (const char * str);
int matches_int (const char * str);
int matches_bool (const char * str);
int matches_char (const char * str);
int matches_double (const char * str);
int matches_func (const char * str);
int matches_maybe (const char * str);
int matches_just (const char * str);
int matches_or (const char * str);
int matches_and (const char * str);
int matches_notoken (const char * str);

/* list functions */
struct token_list * add_token(struct token_list * list,
							  const struct token * tok);
void free_token_list(const struct token_list * list);
void print_token_list(const struct token_list * list);

/* useful macros */
#ifdef DEBUG
/* macro to free a token */
#define __free_token(tok)                       \
		do {                                    \
				free(tok.image);                \
				tok.image = NULL;               \
		} while(0);

#define __free_token_node(node)                 \
		do {                                    \
				free(node);                     \
				node = NULL;                    \
		} while(0);
#else
#define __free_token(tok)                       \
		do {                                    \
				free(tok->image);               \
		} while(0);

#define __free_token_node(node)                 \
		do {                                    \
				free(node);                     \
		} while(0);
#endif

#define __matches_char(str, tok)				\
		do {                                    \
				if (!strlen(str)) return 0;     \
				return *str == tok;             \
		} while (0);

#define __matches_string(str, key)               \
		do {                                     \
				size_t len = strlen(key);        \
				if (!strlen(str) < strlen(key))  \
						return 0;                \
				return !strncmp(str, key, len);  \
		} while (0);

#define IS_ALPHA(a) (a >= 'a' && a <= 'z') \
		|| (a >= 'A' && a <= 'Z')

#define IS_DIGIT(a) a >= '0' && a <= '9'

#define is_whitespace(a) (a == ' ' || a == '\n' || a == '\t')
#endif
