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

#include "gdt.h"
#include "libk/debug.h"

void 
init_descriptor(uint32_t base, uint32_t limit, uint8_t access, GdtDescriptor *self)
{
    self->flags = 0b1010;
    self->access = 0b10000010 | access;

    self->base_low = base & 0xffff;
    self->base_mid = (base & 0xff0000) >> 16;
    self->base_high = (base & 0xff000000) >> 24;

    self->limit_low = limit & 0xffff;
    self->limit_high = (limit & 0xf0000) >> 16;
    
    printk("");
    printk("flags\t\t=\t%b", self->flags);
    printk("access\t\t=\t%08b", self->access);
    printk("base_low\t=\t%b", self->base_low);
    printk("base_mid\t=\t%b", self->base_mid);
    printk("base_high\t=\t%b", self->base_high);
    printk("limit_low\t=\t%b", self->limit_low);
    printk("limit_high\t=\t%b", self->limit_high);    
}

void 
init_gdt(void)
{
    GdtDescriptor desc[GDT_LENGTH];
    GdtPointer ptr;

    init_descriptor(0, 0, 0, &desc[0]);

    init_descriptor(0, 0, KERNEL | DATA, &desc[1]);
    init_descriptor(0, 0, KERNEL | CODE, &desc[2]);

    init_descriptor(0, 0, USER | DATA, &desc[3]);
    init_descriptor(0, 0, USER | CODE, &desc[4]);

    ptr.base = (uintptr_t) &desc[0];
    ptr.limit = sizeof(desc) - 1;

    flush_gdt((uintptr_t) &ptr);
}