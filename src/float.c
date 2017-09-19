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

int matches_float(const char * str, char ** img)
{
        enum states { A, B, C, D, E, F, G, H};
        enum states state = A;
        int multiplier = 1;
        const char * beg = str;

        for (;;) {
                //printf("State %d\n", state );
                switch(state) {
                case A:
                        if (*str == '.'){
                                state = B;
                                str++;
                        }
                        else if (IS_DIGIT(*str)){
                                state = C;
                                str++;
                        }
                        else return 0;
                        break;
                case B:
                        if ( IS_DIGIT(*str)){
                                str++;
                                state = D;
                        }
                        else return 0;
                        break;
                case C:
                        if(IS_DIGIT(*str)){
                                str++;
                        }
                        else if ( *str == 'e' || *str == 'E'){
                                str++;
                                state = F;
                        }
                        else if ( *str == '.'){
                                str++;
                                state = D;
                        }
                        else return (str - beg);
                        break;

                case D:
                        if (IS_DIGIT(*str))
                                str++;
                        else if ( *str == 'e' || *str == 'E'){
                                str++;
                                state = F;
                        }
                        else return (str - beg);
                        break;

                case E:


                        break;

                case F:
                        if ( *str == '-'){
                                str++;
                                state = G;
                        } 
                        else if (IS_DIGIT(*str)){
                                str++;
                                state = H;
                        }
                        else return 0;

                        break;

                case G:
                        if(IS_DIGIT(*str)){
                                str++;
                                state = H;
                        }
                        else return 0;

                        break;

                case H:
                        if(IS_DIGIT(*str))
                                str++;
                        else return (str - beg);

                        break;

                default:
                        break;
                }
        }
}
