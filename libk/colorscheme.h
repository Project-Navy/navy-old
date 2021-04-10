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

#ifndef _LIBK_COLORSCHEME_H_
#define _LIBK_COLORSCHEME_H_

#include <stdint.h>

#define DEFAULT_BG 0x0a0e14
#define DEFAULT_FG 0xb3b1ad


static uint32_t colorscheme[8] =
{
    0xb3b1ad,  /*BLACK   */  
    0xff3333,  /*RED     */
    0xc2d94c,  /*GREEN   */
    0xe6b450,  /*YELLOW  */
    0x39BAE6,  /*BLUE    */
    0xf07178,  /*MAGENTA */
    0x95e6cb,  /*CYAN    */
    0x0a0e14,  /*WHITE   */
};

#endif /* !_LIBK_COLORSCHEME_H_ */
