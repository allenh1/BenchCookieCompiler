#include <bcc/tokens.h>
#include <bcc/scanner.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

int main(int argc, char ** argv)
{
		if (argc != 2) goto error;

		struct token_list * tokens = scan(argv[2]);
		print_token_list(tokens);		
		/* @todo struct ast * ast = build_ast(tokens); */
		/* @todo optimization lvl 1 */
		/* @todo build IR */
		/* @todo optimization lvl 2 */
		/* @todo translation */
		/* @todo fork GCC to assemble binary */

		free_token_list(tokens);
		return 0;

error:
		fprintf(stderr,
				"Invalid args\n"
				"Usage: %s [source]\n",
				argv[0]);
		return 1;
}
