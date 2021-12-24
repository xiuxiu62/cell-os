ASM = nasm
CC = i686-elf-gcc
LD = i686-elf-ld

# IMG = /tmp/cell-os.img
BOOT = ./bin/boot.bin
KERNEL = ./bin/kernel.bin
FILES = ./obj/kernel.s.o

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
	rm -rf ./obj/*

$(KERNEL): $(FILES)
	$(LD) -g -relocatable $(FILES) -o ./obj/kernel-full.o
	$(CC) -T ./src/linker.ld -o ./bin/kernel.bin -ffreestanding -O0 -nostdlib ./obj/kernel-full.o

$(BOOT): ./src/boot.asm 
	$(ASM) -f bin ./src/boot.asm -o $(BOOT) 

$(FILES): ./src/kernel.asm
	$(ASM) -f elf -g ./src/kernel.asm -o $(FILES)

