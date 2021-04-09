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

#ifndef _DEVICES_FRAMEBUFFER_H_
#define _DEVICES_FRAMEBUFFER_H_

#define DEFAULT_BG 0x0a0e14
#define DEFAULT_FG 0xb3b1ad

#define SUCCESS_FG 0xc2d94c

#include <stdint.h>
#include <stddef.h>
#include <stivale2.h>

typedef struct stivale2_struct_tag_framebuffer Framebuffer;

void draw_pixel_fb(Framebuffer *, size_t, size_t, uint32_t);
void clear_fb(Framebuffer *, uint32_t);
void putc_fb(Framebuffer *, uint8_t, size_t, size_t, uint32_t);
void puts_fb(Framebuffer *, const char *, size_t, size_t, uint32_t);

#endif /* !_DEVICES_FRAMEBUFFER_H_ */
