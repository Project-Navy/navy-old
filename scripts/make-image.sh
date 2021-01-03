#!/bin/bash
#
# Copyright (C) 2020-2021 KeyboardSlayer (Jordan Dalcq)
# 
# This file is part of Navy.
# 
# Navy is free software: you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation, either version 3 of the License, or
# (at your option) any later version.
# 
# Navy is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.
# 
# You should have received a copy of the GNU General Public License
# along with Navy.  If not, see <http://www.gnu.org/licenses/>.


path=$(dirname $0)/..
$(dirname $0)/install-limine.sh

dd if=/dev/zero bs=1M count=0 seek=64 of=$path/navy.img
parted -s $path/navy.img mklabel msdos 
parted -s $path/navy.img mkpart primary 2048s 100% 
$path/third-party/echfs/echfs-utils -m -p0 $path/navy.img quick-format 512 
$path/third-party/echfs/echfs-utils -m -p0 $path/navy.img import $(dirname $0)/limine.cfg limine.cfg 
$path/third-party/echfs/echfs-utils -m -p0 $path/navy.img import $path/kernel.elf kernel.elf 
$path/third-party/limine/limine-install $path/third-party/limine/limine.bin $path/navy.img 