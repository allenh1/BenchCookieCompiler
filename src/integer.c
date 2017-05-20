#include <bcc/tokens.h>
#include <bcc/scanner.h>

char * itoa(size_t num)
{
		/* make a string "long enough, probably." */
		char * ret = malloc(21*sizeof(*ret));

		/* we'll build the string backwards */
		char * iter = ret + 20;
		/* get the nearest multiple of ten less than num */
		size_t rounded = num + (10 - num % 10) - 10;
		*(iter--) = num - rounded + '0';
		num = (rounded + 10) / 10;
		rounded = num + (10 - num % 10) - 10;

		for (; rounded > 9; rounded = num + (10 - num % 10) - 10) {
				*(iter--) = ((num - rounded) + '0');
				num /= 10;
		}

		char * real_ret = strdup(iter + 1);
		free(ret);

		return real_ret;
}

/*
 * Check if the string matches an int.
 * If it does, copy the string into img.
 */
int matches_integer(const char * str, char ** img)
{
		enum states { A, B, C, D, E, F, G, ACCEPT };
		enum states state = A;

		const char * beg = str;

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
						} else if (*str == '.') {
								return 0; /* reject */
						} else state = ACCEPT;
						break;
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
						return str - beg;
				}
		} return 0;
}
