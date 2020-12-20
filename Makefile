# Copyright (C) 2020 KeyboardSlayer (Jordan Dalcq)
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


DIRECTORY_GUARD=@mkdir -p $(@D)
BUILD_DIRECTORY := build

CC = x86_64-elf-gcc
LD = x86_64-elf-ld

CFLAGS =							\
	-MD								\
	-Wall							\
	-Werror							\
	-Wextra							\
	-I.								\
	-Ithird-party/limine/stivale	\
	-ffreestanding	             	\
    -g			                 	\
    -nostdlib			         	\
	-mcmodel=kernel                 \

LDFLAGS	= 							\
		  -static					\
 		  -no-pie					\
		  -z max-page-size=0x1000   \
		  -ffreestanding			\
		  -nostdlib					\
		  -Tlink.ld

TARGET = kernel.elf

SRC =								\
	$(wildcard devices/*.c)			\
	$(wildcard kernel/*.c)			\
	$(wildcard libk/*.c)

OBJ = $(patsubst %.c, $(BUILD_DIRECTORY)/%.o, $(SRC))


$(BUILD_DIRECTORY)/%.o: %.c
	$(DIRECTORY_GUARD)
	$(CC) $(CFLAGS) -c $< -o $@

$(TARGET): $(OBJ)
	$(DIRECTORY_GUARD)
	@$(CC) $(LDFLAGS) -o $@ $^
	./scripts/make-image.sh

all: $(TARGET)

run:
	qemu-system-x86_64 -drive format=raw,file=navy.img -serial stdio -m 1G -enable-kvm -display sdl 

clean:
	rm -f kernel.elf
	rm -rf build/
	rm -f navy.img

.PHONY: all
.PHONY: clean

-include $(OBJ:.o=.d)
