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

#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <libk/debug.h>

#include "devices/serial.h"

void printk(const char *format, ...)
{
    va_list ap;
    char str[4096] = {0};
    va_start(ap, format);

    vsnprintf(str, 4096, format, ap);
    puts_serial(COM1, str);
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
