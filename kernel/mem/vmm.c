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

#include <libk/debug.h>
#include <libk/const.h>
#include <libk/range.h>

#include "kernel/macro.h"

#include "vmm.h"

extern uintptr_t kernel_virtual_start;
extern uintptr_t kernel_physical_start;

void 
map_physical(L4PageTable *level4, AddrRange paddr, uintptr_t vaddr_raw, uint8_t flag)
{
    VirtualAddr vaddr;
    size_t page;

    L4Entry l4entry;

    __unused(paddr);

    for (page = 0; range_count_page(paddr); page++)
    {
        vaddr.raw = vaddr_raw + (page * PAGE_SIZE);

        l4entry = level4->entries[vaddr.level4];
        if (!l4entry.present)
        {
            l4entry.present = 1;
            l4entry.write = flag & WRITE;
            l4entry.user = flag & USER_PERM;
            /* l4entry.ptr = */ 
        }

    }

}

uintptr_t 
virtual_to_physical(L4PageTable *level4, uintptr_t vaddr_raw)
{
    VirtualAddr vaddr;

    L4Entry l4entry;

    L3PageTable *level3;
    L3Entry l3entry;

    L2PageTable *level2;
    L2Entry l2entry;

    L1PageTable *level1;
    L1Entry l1entry;

    vaddr.raw = vaddr_raw;

    l4entry = level4->entries[vaddr.level4];
    if (!l4entry.present)
    {
        return 0;
    }

    level3 = (L3PageTable *) l4entry.ptr;
    l3entry = level3->entries[vaddr.level3];
    if (!l3entry.present)
    {
        return 0;
    }

    level2 = (L2PageTable *) l3entry.ptr;
    l2entry = level2->entries[vaddr.level2];
    if (!l2entry.present)
    {
        return 0;
    }

    level1 = (L1PageTable *) l2entry.ptr;
    l1entry = level1->entries[vaddr.level1];
    if (!l1entry.present)
    {
        return 0;
    }

    return l1entry.ptr + vaddr.offset;
}
