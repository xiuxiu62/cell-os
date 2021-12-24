ASM = nasm
CC = i686-elf-gcc
LD = i686-elf-ld

IMG = /tmp/cell-os.img

FILES = ./build/kernel.s.o

BOOT = ./bin/boot.bin
KERNEL = ./bin/kernel.bin

all: ./bin/boot.bin ./bin/kernel.bin
	rm -rf ./bin/os.bin
	dd if=./bin/boot.bin >> ./bin/os.bin
	dd if=./bin/kernel.bin >> ./bin/os.bin
	dd if=/dev/zero bs=512 count=100 >> ./bin/os.bin

$(KERNEL): $(FILES)
	$(LD) -g -relocatable $(FILES) -o ./build/kernel-full.o
	$(CC) -T ./src/linker.ld -o ./bin/kernel.bin -ffreestanding -O0 -nostdlib ./build/kernel-full.o

$(BOOT): ./src/boot.asm 
	$(ASM) -f bin ./src/boot.asm -o $(BOOT) 

$(FILES): ./src/kernel.asm
	$(ASM) -f elf -g ./src/kernel.asm -o $(FILES)

clean:
	rm -rf ./bin/*

# all: 
# 	@if [[ ! -f $(IMG) ]]; then make create_image; fi
# 	@make build
# 	@make install


# create_image:
# 	@printf "Createing disk image: %s\n" $(IMG)
# 	qemu-img create -f qcow2 $(IMG) 1G

# delete_image:
# 	@printf "Deleteing disk image: %s\n" $(IMG)
# 	rm $(IMG)

# build: ./src/*.asm 
# 	@printf "Building bootloader...\n"
# 	@if [[ ! -d "bin" ]]; then mkdir bin; fi
# 	$(ASM) -f bin ./src/boot.asm -o $(BOOT) 
# 	$(ASM) -f bin ./src/kernel.asm -o $(KERNEL) 

# install:
# 	@printf "Writing disk image...\n"
# 	dd if=$(BOOT) of=$(IMG) bs=512 count=1 conv=notrunc
# 	@# dd if=$(LOADER) of=$(IMG) bs=512 count=5 seek=1 conv=notrunc

# run: 
# 	@printf "Booting %s\n" $(IMG)
# 	qemu-system-x86_64 -enable-kvm -drive file=$(IMG),format=raw -cpu host -smp 1 -m 1G 

# clean:
# 	@printf "Removing binaries...\n"
# 	rm -f ./bin/*
# 	@make delete_image
