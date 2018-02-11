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

#include <bcc/tokens.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>
#include <limits.h>
#include <fcntl.h>
#include <float.h>

struct token_list * scan(const char * filename);
struct token * get_next_token(const char ** source);
char * strndup(const char * str, size_t bytes);
char * strdup(const char * str);

/* macro to set the token to a given char */
#define __set_char_tok(str, t)                          \
	do {                                            \
		token->image = strndup((*str)++, 1);    \
		token->tok = t;                         \
	} while (0)

/* like the above, but eat n chars */
#define __set_nlen_tok(str, n, t)                       \
	do {                                            \
		token->image = strndup((*str), n);      \
		token->tok = t;                         \
		*str += n;                              \
	} while (0)

#ifdef DEBUG
#define DBG
#else
#define DBG for (; 0;)
#endif
