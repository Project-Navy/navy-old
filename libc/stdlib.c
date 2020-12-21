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

#include <stdlib.h>
#include <string.h>
#include <stddef.h>
#include <stdint.h>

#include <libk/debug.h>

char *
itoa(int32_t value, char *str, uint16_t base)
{
    size_t index = 0;
    int8_t digit;

    if (value < 0)
    {
        value *= -1;
    }

    do
    {
        digit = value % base;
        value /= base;
        
        if (digit < 0xa)
        {
            str[index++] = digit + '0';
        }

        else
        {
            str[index++] = (digit - 0xa) + 'a';
        }

    }
    while (value);
    
    str = strrev(str);
    
    return str;
}