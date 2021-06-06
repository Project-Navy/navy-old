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

#include "kernel/proc/task.h"

#include "devices/serial.h"
#include "devices/framebuffer.h"

static uint64_t tick = 0;

static const char *exceptions[32] = {
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

uint64_t
fetch_tick(void)
{
    return tick;
}

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

    printf_serial("RAX=%016x RBX=%016x RCX=%016x", stackframe->rax, stackframe->rbx, stackframe->rcx);
    printf_serial("RDX=%016x RSI=%016x RDI=%016x", stackframe->rdx, stackframe->rsi, stackframe->rdi);
    printf_serial("R8=%016x   R9=%016x R10=%016x  ", stackframe->r8, stackframe->r9, stackframe->r10);
    printf_serial("R11=%016x R12=%016x R13=%016x", stackframe->r11, stackframe->r12, stackframe->r13);
    printf_serial("R14=%016x R15=%016x RBP=%016x", stackframe->r14, stackframe->r15, stackframe->rbp);
    printf_serial("\033[91mRIP=%016x\033[0m  CS=%016x FLG=%016x ", stackframe->rip, stackframe->cs, stackframe->rflags);
    printf_serial("RSP=%016x  SS=%016x", stackframe->rsp, stackframe->ss);
    printf_serial("\nCR0=%016x CR2=%016x CR3=%016x", cr0, cr2, cr3);
    printf_serial("CR4=%016x", cr4);

    printf_fb("RAX=%016x RBX=%016x RCX=%016x", stackframe->rax, stackframe->rbx, stackframe->rcx);
    printf_fb("RDX=%016x RSI=%016x RDI=%016x", stackframe->rdx, stackframe->rsi, stackframe->rdi);
    printf_fb("R8=%016x  R9=%016x R10=%016x  ", stackframe->r8, stackframe->r9, stackframe->r10);
    printf_fb("R11=%016x R12=%016x R13=%016x", stackframe->r11, stackframe->r12, stackframe->r13);
    printf_fb("R14=%016x R15=%016x RBP=%016x", stackframe->r14, stackframe->r15, stackframe->rbp);
    printf_fb("\033[31mRIP=%016x\033[0m CS=%016x FLG=%016x ", stackframe->rip, stackframe->cs, stackframe->rflags);
    printf_fb("RSP=%016x SS=%016x", stackframe->rsp, stackframe->ss);
    printf_fb("\nCR0=%016x CR2=%016x CR3=%016x", cr0, cr2, cr3);
    printf_fb("CR4=%016x", cr4);
}

static void
backtrace(uint64_t *rbp)
{
    uint64_t *rip;

    printf_serial("\n=== Backtrace ===");
    printf_fb("\n=== Backtrace ===");

    while (rbp)
    {
        rip = rbp + 1;

        if (*rip)
        {
            printf_serial("0x%x", *rip);
            printf_fb("0x%x", *rip);
        }

        rbp = (uint64_t *) *rbp;
    }

    printf_serial("");
    printf_fb("");
}

uint64_t 
interrupts_handler(uintptr_t rsp)
{
    uint8_t irq;
    
    InterruptStackFrame *stackframe = (InterruptStackFrame *) rsp;

    if (stackframe->intno < 32)
    {
        printf_fb("\n !!! PANIC !!!");
        printf_serial("\n !!! PANIC !!!");

        backtrace(&stackframe->rbp);
        dump_stack_frame(stackframe);

        printf_serial("%s (ERR: %d) (CODE: %d)", exceptions[stackframe->intno], 
                stackframe->intno, stackframe->err);

        printf_fb("%s (ERR: %d) (CODE: %d)", exceptions[stackframe->intno], 
                stackframe->intno, stackframe->err);

        __asm__("cli");
        __asm__("hlt");
    }
    else if (stackframe->intno < 48)
    {
        irq = stackframe->intno - 32;

        if (irq == 0)
        {
            tick++;
            rsp = sched(rsp);
        }
    }

    PIC_sendEOI(stackframe->intno);
    return rsp;
}
