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

#include <stddef.h>
#include <string.h>

#include <libk/const.h>
#include "pmm.h"

static uint8_t *bitmap;

void 
init_pmm(BootInfo *info)
{
    size_t mem = info->memory_usable;
    size_t max_pages = (mem * 1024) / PAGE_SIZE;
    size_t bitmap_size = max_pages / 8;

    if (bitmap_size % 8)
    {
        bitmap_size += 8 - (bitmap_size % 8);
    }

    // TODO: Find an address for the bitmap
}