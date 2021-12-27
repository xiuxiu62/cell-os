ASM = nasm
LD = i686-elf-ld
CC = i686-elf-gcc
CFLAGS = -g -ffreestanding -falign-jumps -falign-functions -falign-labels -falign-loops -fstrength-reduce -fomit-frame-pointer -finline-functions -fno-builtin -Werror -Wno-unused-function -Wno-unused-label -Wno-unused-parameter -Wno-cpp -nostdlib -nostartfiles -nodefaultlibs -Wall -O0 -Iinc 

# IMG = /tmp/cell-os.img
BOOT = ./bin/boot.bin
KERNEL = ./bin/kernel.bin
DIRS = ./bin ./obj ./obj/idt ./obj/io ./obj/mem ./obj/storage
FILES = ./obj/kernel.s.o ./obj/kernel.o ./obj/print.o ./obj/idt/idt.s.o ./obj/idt/idt.o ./obj/io/io.s.o ./obj/mem/memory.o ./obj/mem/heap.o ./obj/mem/kheap.o ./obj/mem/paging.s.o ./obj/mem/paging.o ./obj/storage/disk.o 
INCLUDES = -I./src

.PHONY: clean setup

all: $(BOOT) $(KERNEL)
	@if [[ -f "./bin/os.bin" ]]; then rm -rf ./bin/os.bin; fi
	dd if=./bin/boot.bin >> ./bin/os.bin
	dd if=./bin/kernel.bin >> ./bin/os.bin
	dd if=/dev/zero bs=512 count=100 >> ./bin/os.bin

.ONESHELL:
init:
	@set -eu
	for dir in $(DIRS); do         
		if [[ ! -d $$dir ]]; then  
			echo "Creating $$dir"
			mkdir $$dir 
			touch $$dir/.keep  
		fi                  
	done

build:
	export PREFIX="$$HOME/opt/cross"
	export TARGET=i686-elf
	export PATH="$$PREFIX/bin:$$PATH"
	make clean
	make setup
	make all

run:
	@[[ ! -f "./bin/os.bin" ]] && make build 
	qemu-system-i386 -hda ./bin/os.bin

debug:
	@[[ ! -f "./bin/os.bin" ]] && make build 
	gdb --ex "add-symbol-file ./obj/kernel-full.o" \
		--ex "break kernel.c:51" \
		--ex "target remote | qemu-system-i386 -hda ./bin/os.bin -S -gdb stdio"

build_run:
	make build 
	make run

clean:
	rm -rf ./bin/*
	rm -rf $(FILES)
	rm -rf ./obj/kernel-full.o

$(BOOT): ./src/boot.s 
	$(ASM) -f bin ./src/boot.s -o $(BOOT)

$(KERNEL): $(FILES)
	$(LD) -g -relocatable $(FILES) -o ./obj/kernel-full.o
	$(CC) $(CFLAGS) -T ./src/linker.ld -o ./bin/kernel.bin ./obj/kernel-full.o 

./obj/kernel.s.o: ./src/kernel.s
	$(ASM) -f elf -g ./src/kernel.s -o ./obj/kernel.s.o

./obj/kernel.o: ./src/kernel.c 
	$(CC) $(INCLUDES) $(CFLAGS) -std=gnu99 -c ./src/kernel.c -o ./obj/kernel.o 

./obj/idt/idt.s.o: ./src/idt/idt.s
	$(ASM) -f elf -g ./src/idt/idt.s -o ./obj/idt/idt.s.o

./obj/idt/idt.o: ./src/idt/idt.c
	$(CC) $(INCLUDES) $(CFLAGS) -I./src/idt -std=gnu99 -c ./src/idt/idt.c -o ./obj/idt/idt.o 

./obj/print.o: ./src/print.c
	$(CC) $(INCLUDES) $(CFLAGS) -I./src	-std=gnu99 -c ./src/print.c -o ./obj/print.o 

./obj/io/io.s.o: ./src/io/io.s
	$(ASM) -f elf -g ./src/io/io.s -o ./obj/io/io.s.o

./obj/mem/memory.o: ./src/mem/memory.c
	$(CC) $(INCLUDES) $(CFLAGS) -I./src/mem -std=gnu99 -c ./src/mem/memory.c -o ./obj/mem/memory.o 

./obj/mem/heap.o: ./src/mem/heap.c 
	$(CC) $(INCLUDES) $(CFLAGS) -I./src/mem -std=gnu99 -c ./src/mem/heap.c -o ./obj/mem/heap.o 

./obj/mem/kheap.o: ./src/mem/kheap.c 
	$(CC) $(INCLUDES) $(CFLAGS) -I./src/mem -std=gnu99 -c ./src/mem/kheap.c -o ./obj/mem/kheap.o 

./obj/mem/paging.s.o: ./src/mem/paging.s
	$(ASM) -f elf -g ./src/mem/paging.s -o ./obj/mem/paging.s.o

./obj/mem/paging.o: ./src/mem/paging.c 
	$(CC) $(INCLUDES) $(CFLAGS) -I./src/mem -std=gnu99 -c ./src/mem/paging.c -o ./obj/mem/paging.o 

./obj/storage/disk.o: ./src/storage/disk.c
	$(CC) $(INCLUDES) $(CFLAGS) -I./src/storage -std=gnu99 -c ./src/storage/disk.c -o ./obj/storage/disk.o 
