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

void 
stivale2_parse_header(BootInfo *self, struct stivale2_struct *stivale)
{
    struct stivale2_tag *tag = (struct stivale2_tag *) ((uint64_t) stivale->tags);

    while (tag != NULL)
    {
        switch(tag->identifier)
        {
            case STIVALE2_STRUCT_TAG_RSDP_ID:
                self->rsdp = (uint64_t) ((struct stivale2_struct_tag_rsdp *) tag)->rsdp;
                break;
            default:
                printk("Unknown identifier: 0x%x", tag->identifier);

        }

        tag = (struct stivale2_tag *) ((uintptr_t) tag->next);
    }
}


