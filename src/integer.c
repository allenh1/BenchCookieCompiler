#include <bcc/tokens.h>

char * itoa(size_t num)
{		
		char * ret = calloc(20, sizeof(*ret));
		*ret = '6';		
}

/*
 * Check if the string matches an int.
 * If it does, copy the string into img.
 */
int matches_integer(const char * str, char ** img)
{
		enum states { A, B, C, D, E, F, G, ACCEPT };
		enum states state = A;
		size_t num = 0, base = 10;

		for (;;) {
				switch (state) {
				case A:
						if (*str == '0') {
								str++;
								state = B;
						} else if (IS_DIGIT(*str)) {
								num += *str - '0';
								str++;
								state = C;
						} else return 0;
						break;
				case B:
						if (*str == 'x') {
								str++;
								state = D;
								base = 16;
						} else if (*str == 'b') {
								str++;
								state = E;
								base = 2;
						} else if (IS_DIGIT(*str)) {
								num *= base;
								num += *(str++) - '0';
								state = C;
						} else state = ACCEPT;
						break;
				case C: /* decimal constant */
						if (IS_DIGIT(*str)) {
								num *= base;
								num +=*(str++) - '0';
								state = C;
						} else state = ACCEPT;
				case D: /* hex constant */
						if (is_hex(*str)) {
								num *= base;
								num += base * ((IS_DIGIT(*str)) ?
											   (*(str++) - '0') :
											   ((*(str) >= 'a' && *(str) <= 'f') ?
												(*(str++) - 'a' + 10) :
												(*(str++) - 'A' + 10)));			
								state = G;
						} else return 0;
						break;
				case E: /* binary constant */
						if (is_bin(*str)) {
								num *= base;
								num += base * (*str++);
								state = F;
						} else return 0;
						break;
				case F: /* binary constant */
						if (is_bin(*str)) {
								num *= base;
								num += base * (*str++);
								state = F;
						} else state = ACCEPT;
						break;
				case G: /* hex constant */
						if (is_hex(*str)) {
								num *= base;
								num += base * ((IS_DIGIT(*str)) ?
											   (*(str++) - '0') :
											   ((*str >= 'a' && *str <= 'f') ?
												(*(str++) - 'a' + 10) :
												(*(str++) - 'A' + 10)));
								state = G;
						} else return 0;
						break;
				case ACCEPT: /* acceptor state */
						*img = itoa(num);
						return 1;
				}
		} return 0;
}
