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

#include <libk/debug.h>

#include "kernel/int/interrupt.h"
#include "kernel/int/pic.h"

uintptr_t
interrupts_handler(uintptr_t rsp)
{
    InterruptStackFrame *stackframe = (InterruptStackFrame *) rsp;

    __asm__("sti");
    PIC_sendEOI(stackframe->intno);

    return rsp;
}