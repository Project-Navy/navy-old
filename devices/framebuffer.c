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

#include "framebuffer.h"
#include "serial.h"

#include "libk/escape-state.h"

#include "kernel/font.h"
#include "kernel/colorscheme.h"

FbCFG *config;
EscapeState state = OFF;

size_t ptr = 0;
char nbr[3];

void
init_fb(Framebuffer *self, size_t init_x, uint32_t fg, uint32_t bg)
{
    FbCFG init = {fg, bg, init_x, FONT_SPACE_Y, init_x, self};
    memcpy(config, &init, sizeof(FbCFG));
}

void
draw_pixel_fb(Framebuffer *self, size_t x, size_t y, uint32_t color)
{
    size_t index = x + (self->framebuffer_pitch / sizeof(uint32_t)) * y;
    ((uint32_t *) self->framebuffer_addr)[index] = color;
}

void
putc_fb(Framebuffer *self, uint8_t c, size_t *curx, size_t *cury)
{
    size_t x;
    size_t y;
    size_t offset;

    switch (c)
    {
        case '\n':
        {
            *cury += FONT_SPACE_Y;
            *curx = config->defaultX - FONT_WIDTH;
            return;
        }

        case '\033':
        {
            state = PRE_PARSING; 
            return;
        }
    }

    if (state == PARSING)
    {
        if (c != ';' && c != 'm')
        {
            nbr[ptr++] = c;
            return;
        }

        else {}
    }

    if (state == PRE_PARSING && c == '[')
    {
        state = PARSING;
        return;
    }

    if (state == PARSING && c == 'm')
    {
        int digit = atoi(nbr);
        state = OFF;

        if (digit == 0)
        {
            config->fg = DEFAULT_FG;
            config->bg = DEFAULT_BG;
        }

        if (digit > 29 && digit < 38)
        {
            config->fg = colorscheme[digit-30];
        }
        
        memset(nbr, 0, 3);
        return;
    }

    for (x = 0; x < FONT_WIDTH; x++)
    {
        for (y = 0; y < FONT_HEIGHT; y++)
        {
            if ((font[c][y] >> (FONT_WIDTH - x)) & 1)
            {
                offset = ((y + *cury) * self->framebuffer_pitch) + ((x + *curx) * 4);
                *(uint32_t *)(self->framebuffer_addr + offset) = config->fg;
            }

        }
    }

    *curx += FONT_WIDTH;
}


void
puts_fb(Framebuffer *self, const char *s, size_t *x, size_t *y, size_t defaultX)
{
    size_t i;

    for (i = 0; i < strlen(s); i++)
    {
        if (*y + FONT_HEIGHT >= self->framebuffer_height)
        {
            *y += FONT_SPACE_Y;
            *x = defaultX;
        }

        putc_fb(self, s[i], x, y);
    }
}

void
clear_fb(Framebuffer *self)
{
    size_t x;
    size_t y;

    for (x = 0; x < self->framebuffer_width; x++)
    {
        for (y = 0; y < self->framebuffer_height; y++)
        {
            draw_pixel_fb(self, x, y, config->bg);
        }
    }
}

void
printf_fb(const char *format, ...)
{
    va_list ap;
    char s[4096] = {0};
    va_start(ap, format);

    vsnprintf(s, 4096, format, ap);
    puts_fb(config->fb, s, &config->x, &config->y, config->defaultX);

    config->y += FONT_SPACE_Y;
    config->x = config->defaultX; 

    va_end(ap);
}
