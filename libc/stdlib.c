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

#include <stdlib.h>
#include <string.h>
#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>
#include <math.h>

#include <libk/debug.h>

char * itoa( int64_t value, char * str, uint16_t base )
{
    char * rc;
    char * ptr;
    char * low;
    // Check for supported base.
    if ( base < 2 || base > 36 )
    {
        *str = '\0';
        return str;
    }
    rc = ptr = str;
    // Set '-' for negative decimals.
    if ( value < 0 && base == 10 )
    {
        *ptr++ = '-';
    }
    // Remember where the numbers start.
    low = ptr;
    // The actual conversion.
    do
    {
        // Modulo is negative for negative value. This trick makes abs() unnecessary.
        *ptr++ = "zyxwvutsrqponmlkjihgfedcba9876543210123456789abcdefghijklmnopqrstuvwxyz"[35 + value % base];
        value /= base;
    } while ( value );
    // Terminating the string.
    *ptr-- = '\0';
    // Invert the numbers.
    while ( low < ptr )
    {
        char tmp = *low;
        *low++ = *ptr;
        *ptr-- = tmp;
    }
    return rc;
}


void *
memset(void *s, int c, size_t n)
{
    if (n)
    {
        char *d = (char *) s;
        do 
        {
            *d++ = c;
        } while(--n);
    }

    return s;
}

int
atoi(const char *nptr)
{
    bool is_negative = false;
    int64_t return_value = 0;
    size_t index = 0;

    if (nptr[index] == '-')
    {
        is_negative = true;
        index++;
    }

    while (nptr[index] != '\0')
    {
        uint16_t power = strlen(nptr) - index - 1;

        return_value += (nptr[index] - '0') * (uint32_t) pow(10, power);
        index++;
    }

    if (is_negative)
    {
        return_value *= -1;
    }

    return return_value;
}