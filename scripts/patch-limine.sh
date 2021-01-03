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


cd third-party
to_patch=$(grep -r "x86_64-elf-gcc" | cut -d ":" -f1 | uniq)

for file in $to_patch 
do
    echo "[ * ] Patching $file ..."
    sed -i "s/x86_64-elf-gcc/x86_64-elf-gcc/g" $file 
done