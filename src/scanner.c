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

#include <bcc/scanner.h>

char * strdup(const char * str)
{
	const char * iter;

	for (iter = str; *(iter++); ) ;
	return strndup(str, iter - str);
}

char * strndup(const char * str, size_t bytes)
{
	char * ret = malloc(bytes + 1);
	char * r = ret;

	for (size_t b = bytes; b-- > 0; *(r++) = *(str++)) ;
	ret[bytes] = '\0';
	return ret;
}

struct token * get_next_token(const char ** source)
{
	struct token * token = malloc(sizeof(*token));
	char * tmp = NULL; /* shut up, GCC */
	size_t imgLen = 0;

	/*
	 * If you hate me now, I understand.
	 * just wait till the parser...
	 */


	/*
	 * optimum strategy, filter by ascii ranges, separate single char
	 * tokens from multi char tokens to avoid redudant checks?
	 *     ~Eric B.
	 */
	if (matches_obracket(*source)) {
		__set_char_tok(source, OBRACKET);
	} else if (matches_cbracket(*source)) {
		__set_char_tok(source, CBRACKET);
	} else if (matches_obrace(*source)) {
		__set_char_tok(source, OBRACE);
	} else if (matches_cbrace(*source)) {
		__set_char_tok(source, CBRACE);
	} else if (matches_oparen(*source)) {
		__set_char_tok(source, OPAREN);
	} else if (matches_cparen(*source)) {
		__set_char_tok(source, CPAREN);
	} else if (matches_plus(*source)) {
		__set_char_tok(source, PLUS);
	} else if (matches_minus(*source)) {
		__set_char_tok(source, MINUS);
	} else if (matches_star(*source)) {
		__set_char_tok(source, STAR);
	} else if (matches_semicolon(*source)) {
		__set_char_tok(source, SEMICOLON);
	} else if (matches_underscore(*source)) {
		__set_char_tok(source, UNDERSCORE);
	} else if (matches_bang(*source)) {
		__set_char_tok(source, BANG);
	} else if (matches_func(*source)) {
		__set_nlen_tok(source, 4, FUNC);
	} else if (matches_comma(*source)) {
		__set_char_tok(source, COMMA);
	} else if (matches_fslash(*source)) {
		__set_char_tok(source, FSLASH);
	} else if (matches_bslash(*source)) {
		__set_char_tok(source, BSLASH);
	} else if (matches_ampersand(*source)) {
		__set_char_tok(source, AMPERSAND);
	} else if (matches_pipe(*source)) {
		__set_char_tok(source, PIPE);
	} else if (matches_at(*source)) {
		__set_char_tok(source, AT);
	} else if (matches_quote(*source)) {
		__set_char_tok(source, QUOTE);
	} else if (matches_apostrophe(*source)) {
		__set_char_tok(source, APOSTROPHE);
	} else if (matches_capture(*source)) {
		__set_nlen_tok(source, 7, CAPTURE);
	} else if (matches_maybe(*source)) {
		__set_nlen_tok(source, 5, MAYBE);
	} else if (matches_int(*source)) {
		__set_nlen_tok(source, 3, INT);
	} else if (matches_bool(*source)) {
		__set_nlen_tok(source, 4, BOOL);
	} else if (matches_char(*source)) {
		__set_nlen_tok(source, 4, CHAR);
	} else if (matches_double(*source)) {
		__set_nlen_tok(source, 6, DOUBLE);
	} else if (matches_long(*source)) {
		__set_nlen_tok(source, 4, LONG);
	} else if (matches_unsigned(*source)) {
		__set_nlen_tok(source, 8, UNSIGNED);
	} else if (matches_question(*source)) {
		__set_char_tok(source, QUESTION);
	} else if (matches_and(*source)) {
		__set_nlen_tok(source, 3, AND);
	} else if (matches_or(*source)) {
		__set_nlen_tok(source, 2, OR);
	} else if (matches_just(*source)) {
		__set_nlen_tok(source, 4, JUST);
	} else if (matches_notoken(*source) || (**source) == '\0') {
		__set_char_tok(source, NOTOKEN);
	} else if ((imgLen = matches_integer(*source, &tmp))) {
		token->image = tmp;
		token->tok = INTEGER;
		*source += imgLen;
	} else if ((imgLen = matches_float(*source, &tmp))) {
		__set_nlen_tok(source, imgLen, FLOAT);
	} else {
		fprintf(stderr, "error: invalid token \"%s\"\n", *source);
		return NULL;
	}
	return token;
}

struct token_list * scan(const char * filename)
{
	char * source = NULL;
	FILE * fp = NULL;

	if (NULL == (fp = fopen(filename, "r"))) {
		perror("fopen");
		fprintf(stderr, "Cannot open file '%s'\n", filename);
		exit(1);
	}
	/* read the file */
	if (-1 == fseek(fp, 0, SEEK_END)) {
		perror("fseek");
		exit(2);
	}
	size_t len = ftell(fp);
	if (-1 == fseek(fp, 0, SEEK_SET)) {
		perror("fseek");
		exit(3);
	}
	source = calloc(len + 1, sizeof(char));
	size_t read = fread(source, sizeof(char), len, fp);
	if (len != read) {
		perror("fread");
		fprintf(stderr, "read: %zd\n", read);
		exit(4);
	}
	fclose(fp);
	/* create our list */
	struct token_list * tokens = malloc(sizeof(*tokens));
	tokens->head = NULL;

	/* iterate over our source, gathering tokens */
	struct token * tok = NULL; char * iter = source;
	for (; *iter && (tok = get_next_token((const char**)&iter)); ) {
		if (tok->tok != NOTOKEN) add_token(tokens, tok);
		__free_token(tok);
		free(tok);
	}

	free(source);
	return tokens;
}
