#include <bcc/tokens.h>

/*
 * This function is in its own file...
 * I expect that it will be disgusting,
 * and I will likely never want to look
 * at this hideous thing again.
 *
 * Be gone, print function!
 */

void print_each_node(const struct token_node * iter)
{
		/* stop iterating if we reach the end */
		if (iter == NULL) return;

		/* this is the token to print */
		struct token token = iter->tok;

		/* switch statement from hell */
		switch (token.tok) {
		case OBRACKET:
				printf("{<OBRACKET>: %s}\n", token.image);
				break;
		case CBRACKET:
				printf("{<CBRACKET>: %s}\n", token.image);
				break;
		case OBRACE:
				printf("{<OBRACE>: %s}\n", token.image);
				break;
		case CBRACE:
				printf("{<CBRAKE>: %s}\n", token.image);
				break;
		case OPAREN:
				printf("{<OPAREN>: %s}\n", token.image);
				break;
		case CPAREN:
				printf("{<CPAREN>: %s}\n", token.image);
				break;
		case PLUS:
				printf("{<PLUS>: %s}\n", token.image);
				break;
		case MINUS:
				printf("{<MINUS>: %s}\n", token.image);
				break;
		case STAR:
				printf("{<STAR>: %s}\n", token.image);
				break;
		case SEMICOLON:
				printf("{<SEMICOLON>: %s}\n", token.image);
				break;
		case UNDERSCORE:
				printf("{<UNDERSCORE>: %s}\n", token.image);
				break;
		case BANG:
				printf("{<BANG>: %s}\n", token.image);
				break;
		case FUNC:
				printf("{<FUNC>: %s}\n", token.image);
				break;
		case COMMA:
				printf("{<COMMA>: %s}\n", token.image);
				break;
		case FSLASH:
				printf("{<FSLASH>: %s}\n", token.image);
				break;
		case BSLASH:
				printf("{<BSLASH>: %s}\n", token.image);
				break;
		case AMPERSAND:
				printf("{<AMPERSAND>: %s}\n", token.image);
				break;
		case PIPE:
				printf("{<PIPE>: %s}\n", token.image);
				break;
		case AT:
				printf("{<AT>: %s}\n", token.image);
				break;
		case QUOTE:
				printf("{<QUOTE>: %s}\n", token.image);
				break;
		case APOSTROPHE:
				printf("{<APOSTROPHE>: %s}\n", token.image);
				break;
		case DIGIT:
				printf("{<DIGIT>: %s}\n", token.image);
				break;
		case ALPHA:
				printf("{<ALPHA>: %s}\n", token.image);
				break;
		case INTEGER:
				printf("{<INTEGER>: %s}\n", token.image);
				break;
		case FLOAT:
				printf("{<FLOAT>: %s}\n", token.image);
				break;
		case NOTOKEN:
				printf("{<NOTOKEN>: %s}\n", token.image);
				break;
		case CAPTURE:
				printf("{<CAPTURE>: %s}\n", token.image);
				break;
		case MAYBE:
				printf("{<MAYBE>: %s}\n", token.image);
				break;
		case INT:
				printf("{<INT>: %s}\n", token.image);
				break;
		case BOOL:
				printf("{<BOOL>: %s}\n", token.image);
				break;
		case CHAR:
				printf("{<CHAR>: %s}\n", token.image);
				break;
		case DOUBLE:
				printf("{<DOUBLE>: %s}\n", token.image);
				break;
		case LONG:
				printf("{<LONG>: %s}\n", token.image);
				break;
		case UNSIGNED:
				printf("{<UNSIGNED>: %s}\n", token.image);
				break;
		case QUESTION:
				printf("{<QUESTION>: %s}\n", token.image);
				break;
		case AND:
				printf("{<AND>: %s}\n", token.image);
				break;
		case OR:
				printf("{<OR>: %s}\n", token.image);
				break;
		case JUST:
				printf("{<JUST>: %s}\n", token.image);
				break;
		default:
				fprintf(stderr,
						"error: unknown token!\n"
						" token: %s\n",
						token.image);
				exit(1);
		}

		/* iterate to the next node */
		print_each_node(iter->next);
}

void print_token_list(const struct token_list * list)
{
		/* call helper function */
		print_each_node(list->head);
}
