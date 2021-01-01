/**
 * Copyright (C) 2021 KeyboardSlayer (Jordan Dalcq)
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

#define GDT_LENGTH 5

#include <stdint.h>

typedef struct __attribute__((packed))
{
   uint16_t base_low;
   uint16_t limit_low;
   uint8_t base_mid;

   uint8_t access;
   uint8_t limit_high: 4;
   uint8_t flags: 4;
   uint8_t base_high;
} GdtDescriptor;

typedef struct __attribute__((packed))
{
    uint16_t limit;
    uintptr_t base;
} GdtPointer;

enum GDT_BIT_FIELD
{
    KERNEL     = 0b00000000,
    USER       = 0b01100000,
    CODE       = 0b00011000,
    DATA       = 0b00010000,
};

void init_gdt(void);
void init_descriptor(uint32_t, uint32_t, uint8_t, GdtDescriptor*);

extern void flush_gdt(uintptr_t);

#endif /* !_KERNEL_MEM_GDT_H_ */