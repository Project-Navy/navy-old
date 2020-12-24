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

void 
init_entry(uint32_t base, uint32_t limit, uint8_t access, GdtEntry *entry)
{
    entry->base0_15 = base & 0xffff;
    entry->limit0_15 = base & 0xffff;

    entry->base24_31 = (base & 0xff000000) >> 24;
    entry->limit16_19 = (limit & 0xf0000) >> 16;
    entry->base16_23 = (base & 0xff0000) >> 16;

    entry->flags = 0b1110;
    entry->access = access;
}

void 
init_gdt(void)
{
    GdtEntry entries[GDT_ENTRIES];
    GdtDescriptor descriptor;

    init_entry(0, 0, 0, &entries[0]);   /* NULL DESCRIPTOR */

    init_entry(0, 0xffffffff, PRESENT | KERNEL | DATA | READ_WRITE, &entries[1]); /* KERNEL DATA */
    init_entry(0, 0xffffffff, PRESENT | KERNEL | CODE | EXECUTABLE | READ_WRITE, &entries[2]); /* KERNEL CODE */

    init_entry(0, 0xffffffff, PRESENT | USER | DATA | READ_WRITE, &entries[3]); /* USER DATA */
    init_entry(0, 0xffffffff, PRESENT | USER | CODE | EXECUTABLE | READ_WRITE, &entries[4]); /* USER CODE*/

    descriptor.offset = (uintptr_t) &entries[0];
    descriptor.size = (uint16_t) (sizeof(GdtEntry) * GDT_ENTRIES);

    flush_gdt(&descriptor);
}