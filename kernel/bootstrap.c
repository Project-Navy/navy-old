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
#include "devices/framebuffer.h"
#include "kernel/macro.h"

#include "kernel/mem/pmm.h"
#include "kernel/mem/gdt.h"

#include "kernel/int/idt.h"
#include "kernel/int/pic.h"

typedef uint8_t stack[4096];
static stack stacks[10] = { 0 };

void bootstrap(struct stivale2_struct *);


struct stivale2_header_tag_framebuffer framebuffer_request = {
    .tag = {
        .identifier = STIVALE2_HEADER_TAG_FRAMEBUFFER_ID,
        .next = 0
    },

    .framebuffer_width = 0,
    .framebuffer_height = 0,
    .framebuffer_bpp = 0
};

__attribute__((section(".stivale2hdr"), used))
struct stivale2_header header2 = 
{
    .entry_point = (uint64_t) bootstrap,
    .stack       = (uintptr_t) stacks[0] + sizeof(stack),
    .flags       = 0,
    .tags        = (uintptr_t) &framebuffer_request
};

void 
bootstrap(struct stivale2_struct *stivale)
{
    BootInfo info;

    stivale2_parse_header(&info, stivale);
    printk("%s Total memory: %d MiB", SUCCESS, info.memory_usable / 1048576);

    init_serial(COM1);
    clear_fb(info.framebuffer, DEFAULT_BG);

    init_gdt();
    printk("%s GDT loaded !", SUCCESS);
    puts_fb(info.framebuffer, "[ + ]", 10, 10, SUCCESS_FG);
    puts_fb(info.framebuffer, "GDT loaded !", 56, 10, DEFAULT_FG);

    init_pic();
    printk("%s PIC initialised !", SUCCESS);
    puts_fb(info.framebuffer, "[ + ]", 10, 26, SUCCESS_FG);
    puts_fb(info.framebuffer, "PIC initialised !", 56, 26, DEFAULT_FG);

    init_idt();
    printk("%s IDT loaded !", SUCCESS);
    puts_fb(info.framebuffer, "[ + ]", 10, 42, SUCCESS_FG);
    puts_fb(info.framebuffer, "IDT loaded !", 56, 42, DEFAULT_FG);

    __asm__("int $1");

    for (;;);
}
