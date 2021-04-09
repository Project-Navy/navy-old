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

#ifndef _LIBK_BOOTINFO_H_
#define _LIBK_BOOTINFO_H_

#include <stdint.h>
#include <stivale2.h>

#include <libk/const.h>
#include <libk/range.h>

typedef struct 
{
    AddrRange range;
    uint8_t type;
} MmapEntry;

typedef struct 
{
    uintptr_t epoch;
    uintptr_t rsdp;

    struct stivale2_struct_tag_framebuffer *framebuffer;

    size_t memory_map_size;
    size_t memory_usable; 
    MmapEntry mmap[LIMIT_MEMORY_MAP_SIZE];
} BootInfo;

void stivale2_parse_header(BootInfo *, struct stivale2_struct *);
void stivale2_parse_mmap(BootInfo *, struct stivale2_struct_tag_memmap *);

#endif /* !_LIBK_BOOTINFO_H_ */
