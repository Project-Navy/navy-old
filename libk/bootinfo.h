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

#ifndef _LIBK_BOOTINFO_H_
#define _LIBK_BOOTINFO_H_

#include <stdint.h>

typedef struct 
{
    uint64_t epoch;
    uint64_t rsdp;
} BootInfo;

void stivale2_parse_header(BootInfo *, struct stivale2_struct *);

#endif /* !_LIBK_BOOTINFO_H_ */