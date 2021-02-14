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

#include <stivale2.h>
#include <stdint.h>
#include <stddef.h>
#include <stdlib.h>

#include <libk/debug.h>
#include <libk/bootinfo.h>

#include "devices/serial.h"
#include "kernel/macro.h"

#include "kernel/mem/gdt.h"

#include "kernel/int/idt.h"
#include "kernel/int/pic.h"

typedef uint8_t stack[4096];
static stack stacks[10] = { 0 };

void bootstrap(struct stivale2_struct *);

__attribute__((section(".stivale2hdr"), used))
struct stivale2_header header2 = 
{
    .entry_point = (uint64_t) bootstrap,
    .stack       = (uintptr_t) stacks[0] + sizeof(stack),
    .flags       = 0,
    .tags        = 0
};

void 
bootstrap(struct stivale2_struct *stivale)
{
    BootInfo info;

    init_serial(COM1);

    init_gdt();
    printk("%s GDT loaded !", SUCCESS);

    init_pic();
    printk("%s PIC initialised !", SUCCESS);

    init_idt();
    printk("%s IDT loaded !", SUCCESS);

    stivale2_parse_header(&info, stivale);

    for (;;);
}
