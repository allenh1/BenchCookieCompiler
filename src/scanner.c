#include <bcc/scanner.h>

struct token * get_next_token(const char ** source)
{
		/* @todo */		
		return NULL;
}

struct token_list * scan(const char * filename)
{
		char * source = NULL; char buf[128];
		int fd = -1; size_t bytes_read = 0;
		size_t source_size = 0;

		if ((fd = open(filename, O_RDONLY, 0600)) == -1) {
				perror("open");
				exit(1);
		}

		/* read the file */
		for (; bytes_read = read(fd, buf, sizeof(buf));) {
				source = realloc(source, source_size += bytes_read);
				source = strcat(source, buf);
		}

		/* if source is empty, return NULL */
		if (!source_size) return NULL;

		/* create our list */
		struct token_list * tokens = malloc(sizeof(*tokens));
		tokens->head = NULL;

		/* iterate over our source, gathering tokens */
		struct token * tok;
		for (; tok = get_next_token((const char **) &source);) {
				add_token(tokens, tok);
				__free_token(tok);
		}

		return tokens;
}
