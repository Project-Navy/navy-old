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

#ifndef _KERNEL_INT_IDT_H_
#define _KERNEL_INT_IDT_H_

#define IDT_ENTRY_LENGTH 256

#include <stdint.h>

typedef struct __attribute__((packed))
{
    uint16_t limit;
    uintptr_t base;
} Idtr;

typedef struct __attribute__((packed))
{
   uint16_t offset_1; 
   uint16_t selector; 
   uint8_t ist;       
   uint8_t type_attr; 
   uint16_t offset_2; 
   uint32_t offset_3; 
   uint32_t zero;     
} IdtDesc;

enum type_attr
{
    INTGATE = 0x8e,
    TRAPGATE = 0xef
};

void init_idt(void);
void init_idt_desc(uintptr_t, enum type_attr, IdtDesc *);
extern void flush_idt(uintptr_t);

#endif /* !_KERNEL_INT_IDT_H_ */