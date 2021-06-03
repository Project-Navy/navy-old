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

#include "gdt.h"
#include "libk/debug.h"

static GdtDescriptor desc[GDT_LENGTH];
static GdtPointer ptr;

void 
init_descriptor(uint32_t base, uint32_t limit, uint8_t access, GdtDescriptor *self)
{
    if (access != DATA)
    {
        self->flags = 0b0010;
    }
    else 
    {
        self->flags = 0;
    }

    self->access = 0b10000010 | access;

    self->base_low = base & 0xffff;
    self->base_mid = (base & 0xff0000) >> 16;
    self->base_high = (base & 0xff000000) >> 24;

    self->limit_low = limit & 0xffff;
    self->limit_high = (limit & 0xf0000) >> 16;
}

void 
init_gdt(void)
{
    module("GDT");
    
    init_descriptor(0, 0, 0, &desc[0]);

    init_descriptor(0, 0, KERNEL | CODE, &desc[1]);
    init_descriptor(0, 0, KERNEL | DATA, &desc[2]);

    init_descriptor(0, 0, USER | CODE, &desc[3]);
    init_descriptor(0, 0, USER | DATA, &desc[4]);

    ptr.base = (uintptr_t) &desc[0];
    ptr.limit = sizeof(desc) - 1;

    flush_gdt((uintptr_t) &ptr);
    log_debug(SUCCESS, "");
}
