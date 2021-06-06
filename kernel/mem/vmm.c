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

#include "vmm.h"
#include "pmm.h"
#include "kernel/macro.h"
#include <libk/debug.h>
#include <libk/range.h>
#include <string.h>
#include <stdbool.h>

extern uintptr_t kernel_virtual_start;
extern uintptr_t kernel_physical_start;
extern uintptr_t kernel_physical_end;

static uintptr_t *kernel_plm4;

static uintptr_t *
get_next_level(uintptr_t *current_level, size_t index)
{
    uintptr_t *level;

    if (current_level[index] & 0x1)
    {
        level = (uintptr_t *) ((current_level[index] & 0xfffffffffffff000) + MEM_PHYS_OFFSET);
    }
    else
    {
        level = (uintptr_t *) ((uintptr_t) alloc_pmm(1) + MEM_PHYS_OFFSET);
        memset(level, 0, PAGE_SIZE); 

        current_level[index] = (uintptr_t) ((uintptr_t) level - MEM_PHYS_OFFSET) | 0b111; /* Present + Writeable + User */
    }

    return level;
}

static void
map_vmm(uintptr_t *addr_space, uintptr_t physaddr, uintptr_t vaddr, bool is_user)
{
    size_t plm4_entry = (vaddr & ((size_t) 0x1ff << 39)) >> 39;
    size_t pdpt_entry = (vaddr & ((size_t) 0x1ff << 30)) >> 30;
    size_t pd_entry = (vaddr & ((size_t) 0x1ff << 21)) >> 21;
    size_t pt_entry = (vaddr & ((size_t) 0x1ff << 12)) >> 12;

    uintptr_t *pdpt;
    uintptr_t *pd;
    uintptr_t *pt;

    uint64_t flags = 0x5 | (is_user << 1);

    pdpt = get_next_level(addr_space, plm4_entry);
    pd = get_next_level(pdpt, pdpt_entry);
    pt = get_next_level(pd, pd_entry);

    pt[pt_entry] = physaddr | flags;
}

static void
load_cr3(uintptr_t val)
{
    __asm__ volatile ("mov %%cr3, %0" :: "r" (val));
}

void
init_vmm(BootInfo *info)
{
    module("VMM");

    size_t i;

    kernel_plm4 = alloc_pmm(1);
    memset(kernel_plm4, 0, PAGE_SIZE);

    for (i = 0; i < (0x2000000 / PAGE_SIZE); i++)
    {
        uintptr_t addr = i * PAGE_SIZE;
        map_vmm(kernel_plm4, addr, addr, false);
        map_vmm(kernel_plm4, addr, MEM_PHYS_OFFSET + addr, false);
        map_vmm(kernel_plm4, addr, (uintptr_t) &kernel_virtual_start + addr, false);
    }

    for (i = 0; i < info->memory_map_size; i++)
    {
        size_t j;

        for (j = 0; j < info->mmap[i].range.length / PAGE_SIZE; j++)
        {
            uintptr_t addr = info->mmap[i].range.base + j * PAGE_SIZE;
            map_vmm(kernel_plm4, addr, MEM_PHYS_OFFSET + addr, false);

            if (info->mmap[i].type == STIVALE2_MMAP_KERNEL_AND_MODULES)
            {
                map_vmm(kernel_plm4, addr, (uintptr_t) &kernel_virtual_start + addr, false);
            }
        }
    }

    load_cr3((uintptr_t) kernel_plm4 - MEM_PHYS_OFFSET);

    log_debug(SUCCESS, "");
}    
