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

#ifndef _LIBK_DEBUG_H_
#define _LIBK_DEBUG_H_

#define SUCCESS "\033[32m[+]\033[0m"
#define ERROR "\033[31m[-]\033[0m"

#define assert(__expr) ((__expr) ? (void)(0) : __assert(#__expr, __FILE__, __FUNCTION__, __LINE__))

#include <stdarg.h>

void printk(const char *, ...);
void __assert(const char *, const char *, const char *, int);

#endif /* !_LIBK_DEBUG_H_ */