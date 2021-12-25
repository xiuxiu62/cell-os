ASM = nasm
LD = i686-elf-ld
CC = i686-elf-gcc
CFLAGS = -g -ffreestanding -falign-jumps -falign-functions -falign-labels -falign-loops -fstrength-reduce -fomit-frame-pointer -finline-functions -fno-builtin -Werror -Wno-unused-function -Wno-unused-label -Wno-unused-parameter -Wno-cpp -nostdlib -nostartfiles -nodefaultlibs -Wall -O0 -Iinc 

# IMG = /tmp/cell-os.img
BOOT = ./bin/boot.bin
KERNEL = ./bin/kernel.bin
FILES = ./obj/kernel.s.o ./obj/kernel.o ./obj/std/print.o
INCLUDES = -I./src

.PHONY: clean setup

all: $(BOOT) $(KERNEL)
	@if [[ -f "./bin/os.bin" ]]; then rm -rf ./bin/os.bin; fi
	dd if=./bin/boot.bin >> ./bin/os.bin
	dd if=./bin/kernel.bin >> ./bin/os.bin
	dd if=/dev/zero bs=512 count=100 >> ./bin/os.bin

setup:
	@if [[ ! -d "./bin" ]]; then mkdir bin; fi
	@if [[ ! -d "./obj" ]]; then mkdir obj; fi

clean:
	rm -rf ./bin/*
	rm -rf $(FILES)

$(BOOT): ./src/boot.asm 
	$(ASM) -f bin ./src/boot.asm -o $(BOOT)

$(KERNEL): $(FILES)
	$(LD) -g -relocatable $(FILES) -o ./obj/kernel-full.o
	$(CC) $(CFLAGS) -T ./src/linker.ld -o ./bin/kernel.bin ./obj/kernel-full.o 

./obj/kernel.s.o: ./src/kernel.asm
	$(ASM) -f elf -g ./src/kernel.asm -o ./obj/kernel.s.o

./obj/kernel.o: ./src/kernel.c ./src/std/print.c 
	$(CC) $(INCLUDES) $(CFLAGS) -std=gnu99 -c ./src/kernel.c -o ./obj/kernel.o 

./obj/std/print.o: ./src/std/print.c
	$(CC) $(INCLUDES) $(CFLAGS) -I./src/std -std=gnu99 -c ./src/std/print.c -o ./obj/std/print.o 

