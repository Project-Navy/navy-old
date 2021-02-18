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

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>

#include <libk/bootinfo.h>
#include <libk/range.h>

void init_pmm(BootInfo *);
void set_page_used_pmm(uint64_t);
bool is_page_used(uint64_t);
size_t find_free_pages_pmm(size_t);
void set_page_free_pmm(uint64_t);
AddrRange pmm_alloc(size_t);
void pmm_free(void *addr, size_t size);