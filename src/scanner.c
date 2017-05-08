#include <bcc/scanner.h>

char * strndup(const char * str, size_t bytes)
{
		char * ret = malloc(bytes + 1);
		char * r = ret;
		for (; bytes --> 0; *(r++) = *(str++));
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
				__set_char_tok(*source, OBRACKET);
		} else if (matches_cbracket(*source)) {
				__set_char_tok(*source, CBRACKET);
		} else if (matches_obrace(*source)) {
				__set_char_tok(*source, OBRACE);
		} else if (matches_cbrace(*source)) {
				__set_char_tok(*source, CBRACE);
		} else if (matches_oparen(*source)) {
				__set_char_tok(*source, OPAREN);
		} else if (matches_cparen(*source)) {
				__set_char_tok(*source, CPAREN);
		} else if (matches_plus(*source)) {
				__set_char_tok(*source, PLUS);
		} else if (matches_minus(*source)) {
				__set_char_tok(*source, MINUS);
		} else if (matches_star(*source)) {
				__set_char_tok(*source, STAR);
		} else if (matches_semicolon(*source)) {
				__set_char_tok(*source, SEMICOLON);
		} else if (matches_underscore(*source)) {
				__set_char_tok(*source, UNDERSCORE);
		} else if (matches_notoken(*source) || (**source) == '\0') {
				__set_char_tok(*source, NOTOKEN);
		} else {
				fprintf(stderr, "error: invalid token \"%s\"\n", *source);
				return NULL;
		} return token;
}

struct token_list * scan(const char * filename)
{
		char * source = calloc(128, sizeof(*source));
		int fd = -1; size_t bytes_read = 0;
		size_t source_size = 0; /* 1 for '\0' */

		if ((fd = open(filename, O_RDONLY, 0600)) == -1) {
				perror("open");
				fprintf(stderr, "filename: \"%s\"\n", filename);
				exit(1);
		}

		/* read the file */
		for (; bytes_read = read(fd, source + source_size, 128);) {
				source = realloc(source, source_size += bytes_read);
		} source[source_size] = '\0';

		/* if source is empty, return NULL */
		if (!source_size) return NULL;

		/* create our list */
		struct token_list * tokens = malloc(sizeof(*tokens));
		tokens->head = NULL;

		/* iterate over our source, gathering tokens */
		struct token * tok = NULL; char * iter = source;
		for (; tok = get_next_token((const char **) &iter);) {
				if (tok->tok != NOTOKEN) add_token(tokens, tok);
				__free_token(tok);
		}

		free(source);
		return tokens;
}
