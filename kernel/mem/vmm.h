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

#ifndef _KERNEL_MEM_VIRTUAL_H_
#define _KERNEL_MEM_VIRTUAL_H_

#include <stdint.h>
#include <stdbool.h>

typedef struct 
{
    union 
    {
        uint64_t raw;

        struct 
        {
            uint16_t sign_ext;
            uint16_t level4 : 9;
            uint16_t level3 : 9;
            uint16_t level2 : 9;
            uint16_t level1 : 9;
            uint16_t offset : 12;
        };
    };
} VirtualAddr;

typedef enum 
{
    WRITE = 1 << 1,
    USER_PERM  = 1 << 2,
} Flag;

typedef struct __attribute__((packed))
{
    bool present: 1;
    bool write: 1;
    bool user: 1;
    bool writethr: 1;
    bool cache_disable: 1;
    bool access: 1;
    bool ign0: 1;
    uint8_t zero: 2;
    uint8_t avl: 3;
    uint64_t ptr: 40;
    uint16_t available: 11;
    bool no_exec: 1;
} L4Entry;

typedef struct __attribute__((packed))
{
    bool present: 1;
    bool write: 1;
    bool user: 1;
    bool writethr: 1;
    bool cache_disable: 1;
    bool access: 1;
    bool ign0: 1;
    bool size: 1;
    bool ign1: 1;
    uint8_t avl: 3;
    uint64_t ptr: 40;
    uint16_t available: 11;
    bool no_exec: 1;
} L3Entry;

typedef struct __attribute__((packed))
{
    bool present: 1;
    bool write: 1;
    bool user: 1;
    bool writethr: 1;
    bool cache_disable: 1;
    bool access: 1;
    bool ign0: 1;
    bool size: 1;
    bool ign1: 1;
    uint8_t avl: 3;
    uint64_t ptr: 40;
    uint16_t available: 11;
    bool no_exec: 1;
} L2Entry;

typedef struct __attribute__((packed))
{
    bool present: 1;
    bool write: 1;
    bool user: 1;
    bool writethr: 1;
    bool cache_disable: 1;
    bool access: 1;
    bool dirty: 1;
    bool ign: 1;
    bool global_trans: 1;
    uint8_t avl: 3;
    uint64_t ptr: 40;
    uint16_t available: 11;
    bool no_exec: 1;
} L1Entry;

typedef struct __attribute__((packed))
{
    L4Entry entries[512];
} L4PageTable;

typedef struct __attribute__((packed))
{
    L3Entry entries[512];
} L3PageTable;

typedef struct __attribute__((packed))
{
    L2Entry entries[512];
} L2PageTable;

typedef struct __attribute__((packed))
{
    L1Entry entries[512];
} L1PageTable;

void init_vmm(void);
void load_cr3(L4PageTable *);
uintptr_t virtual_to_physical(L4PageTable *, uintptr_t);

#endif /* _KERNEL_MEM_VIRTUAL_H_ */