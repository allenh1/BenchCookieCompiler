/*
 * Bench Cookie Compiler
 * Copyright (C) 2018  Hunter L. Allen, Eric C. Theller, Eric P. Bond
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

#include <bcc/tokens.h>

int matches_identifier(const char * str, char ** img)
{
	(void)img;
	enum states { A, B };
	enum states state = A;
	const char * beg = str;

	for (;; ) {
		switch (state) {
		case A:
			if (IS_ALPHA(*str)) {
				state = B;
				str++;
			} else if (*str == '_') {
				state = B;
				str++;
			} else return 0;
			break;
		case B:
			if (IS_ALPHA(*str)) {
				str++;
			} else if (IS_DIGIT(*str)) {
				str++;
			} else return str - beg;
			break;
		default:
			break;
		}
	}
	/* should never get here */
	return 0;
}
