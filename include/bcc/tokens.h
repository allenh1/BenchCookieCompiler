/*
 * Bench Cookie Compiler
 * Copyright (C) 2017  Hunter L. Allen, Eric C. Theller, Eric P. Bond
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License along
 * with this program; if not, write to the Free Software Foundation, Inc.,
 * 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.
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
#define NOTOKEN    21
#define CAPTURE    22
#define MAYBE      23
#define INT        24
#define BOOL       25
#define CHAR       26
#define DOUBLE     27
#define LONG       28
#define UNSIGNED   29
#define QUESTION   30
#define AND        31
#define OR         32
#define JUST       33
#define INTEGER    34
#define FLOAT      35
#define NAME       36
#define STRING_LIT 37
#define CHAR_LIT   38
#define DIGIT      39
#define ALPHA      40

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
int matches_capture (const char * str);
int matches_long (const char * str);
int matches_unsigned (const char * str);
int matches_question (const char * question);
int matches_integer (const char * str, char ** img);
int matches_float (const char * str, char ** img);

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
                free(tok->image);               \
                tok->image = NULL;              \
        } while(0);

#define __free_token_node(node)                 \
        do {                                    \
                free(node);                     \
        } while(0);
#else
#define __free_token(tok)                       \
        do {                                    \
                free((tok)->image);             \
        } while(0);

#define __free_token_node(node)                 \
        do {                                    \
                __free_token(&((node)->tok))    \
                free(node);                     \
        } while(0);
#endif

#define __matches_char(str, tok)                \
        do {                                    \
                return *str == tok;             \
        } while (0);

#define __matches_string(str, key)              \
        do {                                    \
                size_t len = strlen(key);       \
                if (strlen(str) < strlen(key))  \
                        return 0;               \
                return !strncmp(str, key, len); \
        } while (0);

#define IS_ALPHA(a) (a >= 'a' && a <= 'z')      \
        || (a >= 'A' && a <= 'Z')

#define IS_DIGIT(a) a >= '0' && a <= '9'

#define is_hex(a) (a >= '0' && a <= '9')        \
        || (a >= 'a' && a <= 'f')               \
        || (a >= 'A' && a <= 'F')

#define is_bin(a) (a == '0' || a == '1')

#define is_whitespace(a) (a == ' ' || a == '\n' || a == '\t')
#endif
