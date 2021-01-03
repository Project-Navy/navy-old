/**
 * Copyright (C) 2020-2021 KeyboardSlayer (Jordan Dalcq)
 * 
 * This file is part of Navy.
 * 
 * Navy is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 * 
 * Navy is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License
 * along with Navy.  If not, see <http://www.gnu.org/licenses/>.
 */

#include <stdbool.h>
#include <stdarg.h>
#include <stdint.h>
#include <stddef.h>
#include <stdlib.h>
#include <string.h>
#include <libk/debug.h>

#include "devices/serial.h"

void 
printk(const char *format, ...)
{
    va_list list;
    char nbr[64];

    size_t padding = 0;
    const char *ptr = format; 
    bool is_parsing = false;

    va_start(list, format);

    while (*ptr)
    {
        memset(nbr, 0, 64);
        if (*ptr == '%')
        {
            is_parsing = true;
            ptr++;
            continue;
        }
        
        if (is_parsing)
        {
            is_parsing = false;
            if (*ptr == '%') 
            {
                putc_serial(COM1, '%');
                ptr++;
            }
            else if (*ptr == '0')
            {
                padding = *++ptr - '0';
            }
            else if (*ptr == 's')
            {
                puts_serial(COM1, (char *) va_arg(list, const char *));
            }
            else if (*ptr == 'd')
            {
                itoa(va_arg(list, int64_t), nbr, 10);

                while (padding && padding - strlen(nbr) > 0)
                {
                    putc_serial(COM1, '0');
                    padding--;
                }

                padding = 0;
                puts_serial(COM1, nbr);
            }
            else if(*ptr == 'x')
            {
                itoa(va_arg(list, int64_t), nbr, 16);
                
                while (padding && padding - strlen(nbr) > 0)
                {
                    putc_serial(COM1, '0');
                    padding--;
                }

                padding = 0; 
                puts_serial(COM1, nbr);
            }
            else if(*ptr == 'b')
            {
                itoa(va_arg(list, int64_t), nbr, 2);
                
                while (padding && padding - strlen(nbr) > 0)
                {
                    putc_serial(COM1, '0');
                    padding--;
                }

                padding = 0; 
                puts_serial(COM1, nbr);
            }
            else if(*ptr == 'c')
            {
                putc_serial(COM1, (char) va_arg(list, int));
            }
        }
        else 
        {
            putc_serial(COM1, *ptr);
        }

        ptr++;
    }

    putc_serial(COM1, '\n');
}

void 
__assert(const char *expr, const char *file, const char *func, int line)
{
    char line_str[64];
    itoa(line, line_str, 10);

    puts_serial(COM1, "Assert failed: ");
    puts_serial(COM1, expr);
    puts_serial(COM1, " in ");
    puts_serial(COM1, file);
    puts_serial(COM1, " (");
    puts_serial(COM1, func);
    puts_serial(COM1, ": line ");
    puts_serial(COM1, line_str);
    puts_serial(COM1, ") \n");
}