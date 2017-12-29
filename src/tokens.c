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

int matches_obracket(const char * str)
{
	__matches_char(str, '[');
}

int matches_cbracket(const char * str)
{
	__matches_char(str, ']');
}

int matches_obrace(const char * str)
{
	__matches_char(str, '{');
}

int matches_cbrace(const char * str)
{
	__matches_char(str, '}');
}

int matches_oparen(const char * str)
{
	__matches_char(str, '(');
}

int matches_cparen(const char * str)
{
	__matches_char(str, ')');
}

int matches_plus(const char * str)
{
	__matches_char(str, '+');
}

int matches_minus(const char * str)
{
	__matches_char(str, '-');
}

int matches_star(const char * str)
{
	__matches_char(str, '*');
}

int matches_semicolon(const char * str)
{
	__matches_char(str, ';');
}

int matches_underscore(const char *str)
{
	__matches_char(str, '_');
}

int matches_bang(const char * str)
{
	__matches_char(str, '!');
}

int matches_comma(const char *str)
{
	__matches_char(str, ',');
}

int matches_fslash(const char * str)
{
	__matches_char(str, '/');
}

int matches_bslash(const char * str)
{
	__matches_char(str, '\\');
}

int matches_ampersand(const char * str)
{
	__matches_char(str, '&');
}

int matches_pipe(const char * str)
{
	__matches_char(str, '|');
}

int matches_at(const char * str)
{
	__matches_char(str, '@');
}

int matches_quote(const char * str)
{
	__matches_char(str, '"');
}

int matches_apostrophe(const char *str)
{
	__matches_char(str, '\'');
}

int matches_question(const char * str)
{
	__matches_char(str, '?');
}

int matches_int(const char * str)
{
	__matches_string(str, "int");
}

int matches_bool(const char * str)
{
	__matches_string(str, "bool");
}

int matches_char(const char * str)
{
	__matches_string(str, "char");
}

int matches_double(const char * str)
{
	__matches_string(str, "double");
}

int matches_func(const char * str)
{
	__matches_string(str, "func");
}

int matches_maybe(const char * str)
{
	__matches_string(str, "maybe");
}

int matches_just(const char * str)
{
	__matches_string(str, "just");
}

int matches_or(const char * str)
{
	__matches_string(str, "or");
}

int matches_and(const char * str)
{
	__matches_string(str, "and");
}

int matches_capture(const char * str)
{
	__matches_string(str, "capture");
}

int matches_long(const char * str)
{
	__matches_string(str, "long");
}

int matches_unsigned(const char * str)
{
	__matches_string(str, "unsigned");
}

int matches_notoken(const char * str)
{
	return *str && is_whitespace(*str);
}

int is_alpha(const char * str)
{
	if (!strlen(str)) return 0;
	return IS_ALPHA(*str);
}

int is_digit(const char * str)
{
	if (!strlen(str)) return 0;
	return IS_DIGIT(*str);
}

/*
 * List functions
 */

struct token_list * add_token(
	struct token_list * list,       /*  list pointer */
	const struct token * tok        /* token pointer */
	)
{
	struct token_node ** iter = &(list->head);

	/* iterate to the end of the list */
	for (; (*iter) != NULL; iter = &(*iter)->next) ;

	/* get length of token */
	size_t len = strlen(tok->image);

	/* allocate new token node */
	*iter = malloc(sizeof(**iter));
	(*iter)->tok.image = malloc(len + 1);

	/* initialize the new node */
	(*iter)->next = NULL;
	strcpy((*iter)->tok.image, tok->image);
	(*iter)->tok.tok = tok->tok;

	/* return the list */
	return list;
}

void free_token_list(const struct token_list * list)
{
	/* don't free an empty token list */
	if (NULL == list) {
		return;
	}

	struct token_node * const * iter = &(list->head);
	struct token_node * const * nxt = NULL;
	for (; *(iter) != NULL; iter = nxt) {
		nxt = &((*iter)->next);
		/* TODO(allenh1): why can't this be free'd? */
		/* free(*iter); */
	}
}
