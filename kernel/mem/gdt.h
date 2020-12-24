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

#ifndef _KERNEL_MEM_GDT_H_
#define _KERNEL_MEM_GDT_H_

#define GDT_ENTRIES 5

#include <stdint.h>

typedef struct __attribute__((__packed__))
{
    uint16_t size;
    uintptr_t offset;
} GdtDescriptor;

typedef struct __attribute__((__packed__))
{
    uint16_t base0_15;
    uint16_t limit0_15;

    uint8_t base24_31;
    uint8_t flags: 4;
    uint8_t limit16_19: 4;
    uint8_t access;
    uint8_t base16_23;
} GdtEntry;

enum GDT_BIT_FIELD
{
    PRESENT    = 0b10000000,
    KERNEL     = 0b00000000,
    USER       = 0b01100000,
    CODE       = 0b00010000,
    DATA       = 0b00010000,
    EXECUTABLE = 0b00001000,
    TSS        = 0b00000000,
    READ_WRITE = 0b00000010,
};

void init_gdt(void);
void init_entry(uint32_t, uint32_t, uint8_t, GdtEntry *);

extern void flush_gdt();    /* TODO */

#endif