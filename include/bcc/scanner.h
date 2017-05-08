#include <bcc/tokens.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>
#include <limits.h>
#include <fcntl.h>

struct token_list * scan(const char * filename);
struct token * get_next_token(const char ** source);
char * strndup(const char * str, size_t bytes);

/* macro to set the token to a given char */
#define __set_char_tok(str, t)					\
	do {										\
		token->image = strndup((*str)++, 1);	\
		token->tok   = t;						\
	} while (0);
