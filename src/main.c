/*
 * Bench Cookie Compiler
 * Copyright (C) 2017  Hunter L. Allen, Eric C. Theller, Eric P. Bond
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
#include <bcc/scanner.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

int main(int argc, char ** argv)
{
        if (argc != 2) goto error;

        struct token_list * tokens = scan(argv[1]);
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
