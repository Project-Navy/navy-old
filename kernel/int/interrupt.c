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

#include "kernel/int/interrupt.h"
#include "kernel/int/pic.h"

const char *exceptions[32] = {
    "Division by zero",
    "Debug",
    "Non-maskable Interrupt",
    "Breakpoint",
    "Overflow",
    "Bound Range Exceeded",
    "Invalid Opcode",
    "Device Not Available",
    "Double Fault",
    "Coprocessor Segment Overrun",
    "Invalid TSS",
    "Segment Not Present",
    "Stack-Segment Fault",
    "General Protection Fault",
    "Page Fault",
    "Reserved",
    "x87 Floating-Point Exception",
    "Aligment Check",
    "Machine Check",
    "SIMD Floating-Point Exception",
    "Virtualization Exception" "Reserved",
    "Reserved",
    "Reserved",
    "Reserved",
    "Reserved",
    "Reserved",
    "Reserved",
    "Reserved",
    "Reserved",
    "Security Exception",
    "Reserved"
};

void 
dump_stack_frame(InterruptStackFrame *stackframe)
{

    uint64_t cr0;
    uint64_t cr2;
    uint64_t cr3;
    uint64_t cr4;

    __asm__ volatile ("mov %%cr0, %0":"=r" (cr0));
    __asm__ volatile ("mov %%cr2, %0":"=r" (cr2));
    __asm__ volatile ("mov %%cr3, %0":"=r" (cr3));
    __asm__ volatile ("mov %%cr4, %0":"=r" (cr4));

    printk("RAX=%016x RBX=%016x RCX=%016x", stackframe->rax, stackframe->rbx, stackframe->rcx);
    printk("RDX=%016x RSI=%016x RDI=%016x", stackframe->rdx, stackframe->rsi, stackframe->rdi);
    printk("R8=%016x   R9=%016x R10=%016x  ", stackframe->r8, stackframe->r9, stackframe->r10);
    printk("R11=%016x R12=%016x R13=%016x", stackframe->r11, stackframe->r12, stackframe->r13);
    printk("R14=%016x R15=%016x RBP=%016x", stackframe->r14, stackframe->r15, stackframe->rbp);
    printk("\033[91mRIP=%016x\033[0m  CS=%016x FLG=%016x ", stackframe->rip, stackframe->cs, stackframe->rflags);
    printk("RSP=%016x  SS=%016x", stackframe->rsp, stackframe->ss);
    printk("\nCR0=%016x CR2=%016x CR3=%016x", cr0, cr2, cr3);
    printk("CR4=%016x", cr4);
}

void
interrupts_handler(uintptr_t rsp)
{
    InterruptStackFrame *stackframe = (InterruptStackFrame *) rsp;
    PIC_sendEOI(stackframe->intno);

    if (stackframe->intno < 32)
    {
        printk("\n\n");
        dump_stack_frame(stackframe);

        printk("%s %s (ERR: %d) (CODE: %d)", ERROR, exceptions[stackframe->intno], 
                stackframe->intno, stackframe->err);
    }

    __asm__("cli");
    __asm__("hlt");
}