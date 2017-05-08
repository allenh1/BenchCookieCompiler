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
		struct token_list * list, /*  list pointer */
		const struct token * tok  /* token pointer */
		)
{
		struct token_node ** iter = &(list->head);
		/* iterate to the end of the list */
		for (; (*iter) != NULL; iter = &(*iter)->next);

		/* get length of token */
		size_t len = strlen(tok->image);

		/* allocate new token node */
		assert(*iter = malloc(sizeof(**iter)));
		assert((*iter)->tok.image = malloc(len));

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
		assert(list != NULL);

		struct token_node * const * iter = &(list->head);
		for (; *(iter) != NULL; iter = &(*iter)->next)
				__free_token_node(*iter);
}
