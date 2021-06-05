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

#include <libk/range.h>
#include <libk/const.h>


bool 
is_page_aligned(AddrRange self)
{
    bool is_aligned = (self.base % PAGE_SIZE == 0) && (self.length % PAGE_SIZE == 0);
    return is_aligned;
}

void 
align_range(AddrRange *self)
{
    size_t align = self->base % PAGE_SIZE;

    self->base -= align;
    self->length += align;

    self->length += PAGE_SIZE - self->length % PAGE_SIZE;
}

size_t 
range_count_page(AddrRange self)
{
    if (!is_page_aligned(self))
    {
        align_range(&self);
    }

    return self.length / PAGE_SIZE;
}
