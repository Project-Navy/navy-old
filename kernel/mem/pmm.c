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

#include <string.h>
#include <stivale2.h>

#include <libk/debug.h>
#include <libk/const.h>
#include "pmm.h"
#include "kernel/macro.h"

size_t memory_pages;
uint8_t *bitmap;

void 
init_pmm(BootInfo *info)
{
    size_t i;
    uint8_t bitmap_bytes;
    AddrRange mmap_range = info->mmap[info->memory_map_size-1].range;
    memory_pages = range_count_page(mmap_range);
    bitmap_bytes = (memory_pages + 8 - 1) / 8;

    bitmap = (void *) 0;

    for (i = 1; i < info->memory_map_size; i++)
    {
        if (info->mmap[i].type == STIVALE2_MMAP_USABLE)
        {
            if (info->mmap[i].range.length >= bitmap_bytes)
            {
                bitmap = (void *) (info->mmap[i].range.base + KERN_POS);
            }
        }
    }

    assert(bitmap);
    memset(bitmap, 0xff, bitmap_bytes);


    for (i = 1; i < info->memory_map_size; i++)
    {
        if (info->mmap[i].type == STIVALE2_MMAP_USABLE)
        {
            size_t j;
            
            if (info->mmap[i].range.base == (uintptr_t) bitmap)
            {
                uint64_t bitmap_end_byte;
                uint64_t bitmap_end_page;
                uint64_t entry_end_page;

                bitmap_end_byte = (uintptr_t) bitmap + bitmap_bytes;
                bitmap_end_page = __align(bitmap_end_byte);
                entry_end_page = range_count_page(info->mmap[i].range);

                for (j = bitmap_end_byte; j < entry_end_page; j++)
                {
                    set_page_free_pmm(j);
                }
            }
            else 
            {
                AddrRange aligned = info->mmap[i].range;
                align_range(&aligned);

                for (j = 0; j < aligned.length; j++)
                {
                    set_page_free_pmm(aligned.base + j);
                }
            }
        }
    }
}

void 
set_page_used_pmm(uint64_t page)
{
    uint64_t byte = page / 8;
    uint64_t bit = page % 8;
    bitmap[byte] |= (1 << bit);
}

void 
set_page_free_pmm(uint64_t page)
{
    uint64_t byte = page / 8;
    uint64_t bit = page % 8;
    bitmap[byte] &= ~(1 << bit);
}

bool 
is_page_used(uint64_t page)
{
    uint64_t byte = page / 8;
    uint64_t bit = page % 8;

    return (bitmap[byte] & (1 << bit)) >> bit;
}

size_t
find_free_pages_pmm(size_t size)
{
    size_t i;
    size_t page_needed = __align(size);
    size_t page_found = 0;
    size_t current_page = 0;

    for (i = 0; i < memory_pages; i++)
    {
        if (!is_page_used(i))
        {
            if (page_found == 0)
            {
                current_page = i;
            }

            page_found++;
        }
        else 
        {
            page_found = 0;
        }

        if (page_found >= page_needed)
        {
            return current_page;
        }
    }

    assert(0);
    return 0;
}

AddrRange
pmm_alloc(size_t size)
{
    size_t i;
    AddrRange addr;
    uint64_t page_needed = __align(size);
    uint64_t free_page = find_free_pages_pmm(size);

    addr.base = free_page;
    addr.length = page_needed;

    for (i = 0; i < page_needed; i++)
    {
        set_page_used_pmm(free_page + i);
    }

    return addr;
}

void 
pmm_free(void *addr, size_t size)
{
    uint64_t i;

    uint64_t page = (uint64_t) addr / PAGE_SIZE;
    uint64_t pages = __align(size);

    for (i = 0; i < pages; i++)
    {
        set_page_free_pmm(page + i);
    }
}