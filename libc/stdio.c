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

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <libk/debug.h>

int
vsnprintf(char *str, size_t size, const char *format, va_list ap)
{
    char nbr[64];
    size_t padding = 0;
    char padding_str[64];

    bool is_parsing = false;
    size_t ptr = 0;

    while (*format || ptr > size)
    {
        memset(nbr, 0, 64);
        if (*format == '%')
        {
            is_parsing = true;
            format++;
            continue;
        }

        if (is_parsing)
        {
            is_parsing = false;
            if (*format == '%')
            {
                str[ptr++] = '%';
                format++;
            }
            else if (*format == '0')
            {
                size_t i = 0;
                format++;

                while (*format >= '0' && *format <= '9')
                {
                    padding_str[i++] = *format++; 
                }

                padding_str[i] = '\0';
                padding = atoi(padding_str);

                is_parsing = true;
                continue;
            }
            else if (*format == 's')
            {
                const char *s = va_arg(ap, const char *);
                ptr += strlen(s);
                strcat(str, s);
            }
            else if (*format == 'd')
            {
                itoa(va_arg(ap, int64_t), nbr, 10);
                while (padding && padding - strlen(nbr) > 0)
                {
                    str[ptr++] = '0';
                    padding--;
                }

                padding = 0;
                ptr += strlen(nbr);
                strcat(str, nbr);
            }
            else if (*format == 'x')
            {
                itoa(va_arg(ap, int64_t), nbr, 16);

                while (padding && padding - strlen(nbr) > 0)
                {
                    str[ptr++] = '0';
                    padding--;
                }

                padding = 0;
                ptr += strlen(nbr);
                strcat(str, nbr);
            }
            else if (*format == 'b')
            {
                itoa(va_arg(ap, int64_t), nbr, 2);

                while (padding && padding - strlen(nbr) > 0)
                {
                    str[ptr++] = '0';
                    padding--;
                }

                padding = 0;
                ptr += strlen(nbr);
                strcat(str, nbr);
            }
            else if (*format == 'c')
            {
                str[ptr++] = (char) va_arg(ap, int);
            }
        }
        else
        {
            str[ptr++] = *format;
        }

        format++;
    }

    return 0;
}
