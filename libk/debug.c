/**
 * Copyright (C) 2020 KeyboardSlayer (Jordan Dalcq)
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
#include <stddef.h>
#include <libk/debug.h>

#include "devices/serial.h"

void 
printk(const char *format, ...)
{
    va_list list;
    size_t padding;

    const char *ptr = format; 
    bool is_parsing = false;

    va_start(list, format);
    (void) padding;

    while (*ptr)
    {
        if (*ptr == '%')
        {
            is_parsing = true;
            ptr++;
            continue;
        }
        

        if (is_parsing)
        {
            if (*ptr == '%') 
            {
                putc_serial(COM1, '%');
                is_parsing = false;
                ptr++;
            }
            else if (*ptr == '0')
            {
                padding = *++ptr - '0';
                ptr++;    
            }
            else if (*ptr == 's')
            {
                puts_serial(COM1, (char *) va_arg(list, const char *));
                is_parsing = false;
            }
        }
        else 
        {
            putc_serial(COM1, *ptr);
        }

        if (*ptr)
        {
            ptr++;
        }
    }
}