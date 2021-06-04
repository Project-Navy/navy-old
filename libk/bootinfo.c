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
#include <stivale2.h>

#include <libk/bootinfo.h>
#include <libk/debug.h>
#include "devices/serial.h"

void 
stivale2_parse_mmap(BootInfo *self, struct stivale2_struct_tag_memmap *mmap)
{
    AddrRange range;
    MmapEntry *entry;
    struct stivale2_mmap_entry mmap_entry;
    size_t i;

    for (i = 0; i < mmap->entries; i++)
    {
        mmap_entry = mmap->memmap[i];

        if (mmap_entry.base > UINTPTR_MAX || (mmap_entry.base + mmap_entry.length > UINTPTR_MAX))
        {
            continue;
        }

        if (mmap_entry.type == STIVALE2_MMAP_USABLE)
        {
            self->memory_usable += mmap_entry.length;
        }

        self->usable_pages = self->memory_usable / 4096
        entry = &self->mmap[self->memory_map_size];
        
        range.base = mmap_entry.base;
        range.length = mmap_entry.length;

        align_range(&range);
        entry->range = range;
        entry->type = mmap_entry.type;

        self->memory_map_size++;
    }
}

void 
stivale2_parse_header(BootInfo *self, struct stivale2_struct *stivale) 
{
    struct stivale2_tag *tag = (struct stivale2_tag *) ((uint64_t) stivale->tags);

    self->memory_usable = 0;
    self->memory_map_size = 0;
    self->usable_pages = 0;

    while (tag != NULL)
    {
        switch(tag->identifier)
        {
            case STIVALE2_STRUCT_TAG_RSDP_ID:
                self->rsdp = (uintptr_t) ((struct stivale2_struct_tag_rsdp *) tag)->rsdp;
                break;
            case STIVALE2_STRUCT_TAG_EPOCH_ID:
                self->epoch = (uintptr_t) ((struct stivale2_struct_tag_epoch *) tag)->epoch;
                break;
            case STIVALE2_STRUCT_TAG_FRAMEBUFFER_ID:
                self->framebuffer = (struct stivale2_struct_tag_framebuffer *) tag;
                break;
            case STIVALE2_STRUCT_TAG_MEMMAP_ID:
                stivale2_parse_mmap(self, (struct stivale2_struct_tag_memmap *) tag);
            default:
                printf_serial("Unknown identifier: 0x%x", tag->identifier);
        }

        tag = (struct stivale2_tag *) ((uintptr_t) tag->next);
    }
}


