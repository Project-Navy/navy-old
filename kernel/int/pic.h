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

#ifndef _NAVY_X86_PIC_H_
#define _NAVY_X86_PIC_H_

#define MASTER_CMD      0x20
#define MASTER_DATA     0x21
#define MASTER_OFFSET   0x20

#define SLAVE_CMD       0xa0
#define SLAVE_DATA      0xa1
#define SLAVE_OFFSET    0x28

#include <stdint.h>

void init_pic(void);
void PIC_sendEOI(int);

#endif