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

#include <stddef.h>
#include <libk/debug.h>

#include "libc/stdlib.h"
#include "kernel/int/idt.h"

extern uintptr_t __interrupt_vector[];

IdtDesc kidt[IDT_ENTRY_LENGTH] = {};
Idtr kidtr;

void 
init_idt_desc(uintptr_t offset, enum type_attr type, IdtDesc *self)
{
    self->offset_1 = offset & 0xffff;
    self->offset_2 = (offset >> 16) & 0xffff;
    self->offset_3 = (offset >> 32) & 0xffffffff;

    self->ist = 0;
    self->selector = 0x08;
    self->type_attr = type;
    self->zero = 0;
}

void 
init_idt(void)
{
    size_t i;
    memset(kidt, 0, 256 * sizeof(IdtDesc));

    for (i = 0; i < 3; i++)
    {
        init_idt_desc(__interrupt_vector[i], INTGATE, &kidt[i]);
    }

    init_idt_desc(__interrupt_vector[3], TRAPGATE, &kidt[3]);
    init_idt_desc(__interrupt_vector[4], TRAPGATE, &kidt[4]);

    for (i = 5; i < 48; i++)
    {
        init_idt_desc(__interrupt_vector[i], INTGATE, &kidt[i]);
    }

    kidtr.base = (uintptr_t) &kidt[0];
    kidtr.limit = sizeof(kidt) - 1;

    flush_idt((uintptr_t) &kidtr);
}