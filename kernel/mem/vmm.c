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

uintptr_t *
get_krnl_addrspace(void)
{
    return kernel_plm4;
}

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

uintptr_t *
alloc_plm4(void)
{
    uintptr_t *plm4 = alloc_pmm(1);
    memset(plm4, 0, PAGE_SIZE);

    return plm4;
}

static bool
is_present_vmm(uintptr_t *addrspace, uintptr_t vaddr)
{
    size_t plm4_entry = (vaddr & ((size_t) 0x1ff << 39)) >> 39;
    size_t pdpt_entry = (vaddr & ((size_t) 0x1ff << 30)) >> 30;
    size_t pd_entry = (vaddr & ((size_t) 0x1ff << 21)) >> 21;
    size_t pt_entry = (vaddr & ((size_t) 0x1ff << 12)) >> 12;         

    uintptr_t *pdpt;
    uintptr_t *pd;
    uintptr_t *pt;

    if (addrspace[plm4_entry] & 0x1)
    {
        return false;
    }

    pdpt = get_next_level(addrspace, plm4_entry);

    if (pdpt[pdpt_entry] & 0x1)
    {
        return false;
    }

    pd = get_next_level(pdpt, pdpt_entry);

    if (pd[pd_entry] & 0x1)
    {
        return false;
    }

    pt = get_next_level(pd, pd_entry);

    return pt[pt_entry] & 0x1;
}

static uintptr_t
virt2phys(uintptr_t *addrspace, uintptr_t vaddr)
{
    size_t plm4_entry = (vaddr & ((size_t) 0x1ff << 39)) >> 39;
    size_t pdpt_entry = (vaddr & ((size_t) 0x1ff << 30)) >> 30;
    size_t pd_entry = (vaddr & ((size_t) 0x1ff << 21)) >> 21;
    size_t pt_entry = (vaddr & ((size_t) 0x1ff << 12)) >> 12;         

    uintptr_t *pdpt;
    uintptr_t *pd;
    uintptr_t *pt;

    if (addrspace[plm4_entry] & 0x1)
    {
        return 0; 
    }

    pdpt = get_next_level(addrspace, plm4_entry);

    if (pdpt[pdpt_entry] & 0x1)
    {
        return 0;
    }

    pd = get_next_level(pdpt, pdpt_entry);

    if (pd[pd_entry] & 0x1)
    {
        return 0;
    }

    pt = get_next_level(pd, pd_entry);
    return pt[pt_entry] & 0xfffffffffffff000;
}

static void
unmap_vmm(uintptr_t *addrspace, AddrRange range)
{
    size_t i;
    uintptr_t vaddr;

    uintptr_t *pdpt;
    uintptr_t *pd;
    uintptr_t *pt;

    size_t plm4_entry;
    size_t pdpt_entry;
    size_t pd_entry;
    size_t pt_entry;

    for (i = 0; i < range.base / PAGE_SIZE; i++)
    {
        vaddr = range.base + i * PAGE_SIZE;

        plm4_entry = (vaddr & ((size_t) 0x1ff << 39)) >> 39;
        pdpt_entry = (vaddr & ((size_t) 0x1ff << 30)) >> 30;
        pd_entry = (vaddr & ((size_t) 0x1ff << 21)) >> 21;
        pt_entry = (vaddr & ((size_t) 0x1ff << 12)) >> 12;         

        if (addrspace[plm4_entry] & 0x1)
        {
            continue;
        }

        pdpt = get_next_level(addrspace, plm4_entry);

        if (pdpt[pdpt_entry] & 0x1)
        {
            continue;
        }

        pd = get_next_level(addrspace, pdpt_entry);

        if (pd[pd_entry] & 0x1)
        {
            continue;
        }

        pt = get_next_level(pd, pd_entry);
        pt[pt_entry] = 0;
    }
}

void
free_vmm(void *addrspace, AddrRange range)
{
    size_t i;
    uintptr_t vaddr;
    AddrRange physRange;
    AddrRange virtRange;
    
    assert(is_page_aligned(range));
    
    for (i = 0; i < range.length / PAGE_SIZE; i++)
    {
        vaddr = range.base + i * PAGE_SIZE;

        if (is_present_vmm(addrspace, vaddr))
        {
            physRange = (AddrRange) {
                .base = virt2phys(addrspace, vaddr),
                .length = PAGE_SIZE
            };

            virtRange = (AddrRange) {
                .base = vaddr,
                .length = PAGE_SIZE
            };

            free_pmm(physRange);
            unmap_vmm(addrspace, range);
        }
    }
}

void
alloc_vmm(void *addrspace, size_t pages, uintptr_t *vaddr, bool is_user)
{
    size_t i;
    size_t start;
    size_t end;
    size_t current_size = 0;

    AddrRange range =
    {
        .base = (uintptr_t) alloc_pmm(pages),
        .length = pages * PAGE_SIZE
    };

    align_range(&range);
    assert(is_page_aligned(range));

    if (is_user)
    {
        start = 256;
        end = 1024;
    }
    else
    {
        start = 1;
        end = 256;
    }

    for (i = start * 1024; i < end * 1024; i++)
    {
        uintptr_t addr = i * PAGE_SIZE;

        if (!is_present_vmm(addrspace, addr))
        {
            if (current_size == 0)
            {
                *vaddr = addr;
            }

            current_size += PAGE_SIZE;

            if (current_size == range.length)
            {
                map_vmm(addrspace, range.base, *vaddr, is_user);
                return;
            }
        }
    }

    module("VMM");
    log_debug(ERROR, "Wanted: %d, Got: %d", range.length, current_size);
    assert(0);
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
