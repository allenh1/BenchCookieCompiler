#include <bcc/scanner.h>

char * strndup(const char * str, size_t bytes)
{
		char * ret = malloc(bytes + 1);
		char * r = ret;
		for (size_t b = bytes; b --> 0; *(r++) = *(str++));
		ret[bytes] = '\0';
		return ret;
}

struct token * get_next_token(const char ** source)
{
		struct token * token = malloc(sizeof(*token));

		/*
		 * If you hate me now, I understand.
		 * just wait till the parser...
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
		} else {
				fprintf(stderr, "error: invalid token \"%s\"\n", *source);
				return NULL;
		} return token;
}

struct token_list * scan(const char * filename)
{
		char * source = calloc(129, sizeof(char));
		int fd = -1; size_t bytes_read = 0;
		size_t source_size = 0;

		if ((fd = open(filename, O_RDONLY, 0600)) == -1) {
				perror("open");
				fprintf(stderr, "filename: \"%s\"\n", filename);
				exit(1);
		}

		/* read the file */
		for (; bytes_read = read(fd, source + source_size, 128);) {
				source = realloc(source, source_size += bytes_read);
		} source[source_size++] = '\0';

		/* if source is empty, return NULL */
		if (!source_size) return NULL;

		/* create our list */
		struct token_list * tokens = malloc(sizeof(*tokens));
		tokens->head = NULL;

		/* iterate over our source, gathering tokens */
		struct token * tok = NULL; char * iter = source;
		for (; *iter && (tok = get_next_token((const char **) &iter));) {
				if (tok->tok != NOTOKEN) add_token(tokens, tok);
				__free_token(tok);
		}

		free(source);
		return tokens;
}
