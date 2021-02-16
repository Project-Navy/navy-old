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

#include "virtual.h"

static L4PageTable table4 __attribute__((aligned(4096)));
static L3PageTable table3 __attribute__((aligned(4096)));
static L2PageTable table2 __attribute__((aligned(4096)));
static L1PageTable table1 __attribute__((aligned(4096)));

uintptr_t 
virtual_to_physical(uintptr_t vaddr_raw)
{
    VirtualAddr vaddr;
    L4Entry l4entry;
    L3Entry l3entry;
    L2Entry l2entry;
    L1Entry l1entry;

    vaddr.raw = vaddr_raw;

    l4entry = table4.entries[vaddr.level4];
    if (!l4entry.present)
    {
        return 0;
    }

    l3entry = table3.entries[vaddr.level3];
    if (!l3entry.present)
    {
        return 0;
    }

    l2entry = table2.entries[vaddr.level2];
    if (!l2entry.present)
    {
        return 0;
    }

    l1entry = table1.entries[vaddr.level1];
    if (!l1entry.present)
    {
        return 0;
    }

    return vaddr.offset + l1entry.ptr;
}
