UNAME_S := $(shell uname -s)
CC=i386-elf-gcc
LD=i386-elf-ld

ifeq ($(UNAME_S),Linux)
CFLAGS = -ffreestanding -I/usr/lib/gcc/i386-elf/15.1.0/include/ -g -Wall -Wextra \
         -fno-exceptions -m32 -nostdlib -nostdinc -fno-stack-protector \
         -fno-builtin-function -fno-builtin

main: start bootloader kernel concat qemu

start:
	rm -rf ./bin
	mkdir -p ./bin

bootloader:
	nasm -f bin src/boot/boot.asm -o bin/bootloader.bin

kernel:
	$(CC) $(CFLAGS) -c src/kernel/kernel.c -o ./bin/kernel.c.o
	$(CC) $(CFLAGS) -c src/kernel/vga.c -o ./bin/vga.c.o
	$(CC) $(CFLAGS) -c src/kernel/idt.c -o ./bin/idt.c.o
	$(CC) $(CFLAGS) -c src/kernel/pic.c -o ./bin/pic.c.o
	nasm -f elf src/kernel/kernelstrap.asm -o ./bin/kernelstrap.asm.o

	# First, link with symbols into ELF (for debugging / objdump)
	$(LD) -m elf_i386 -o ./bin/kernel.elf -Ttext 0x1000 \
		./bin/kernelstrap.asm.o \
		./bin/vga.c.o \
		./bin/idt.c.o \
		./bin/pic.c.o \
		./bin/kernel.c.o

	# Then, produce the flat binary for booting
	$(LD) -m elf_i386 --oformat binary -o ./bin/kernel.bin -Ttext 0x1000 \
		./bin/kernelstrap.asm.o \
		./bin/vga.c.o \
		./bin/idt.c.o \
		./bin/pic.c.o \
		./bin/kernel.c.o 

objdump:
	$(OBJDUMP) -d ./bin/kernel.elf > ./bin/kernel.asm

concat:
	cat ./bin/bootloader.bin ./bin/kernel.bin > ./bin/os.bin

qemu:
	qemu-system-x86_64 -drive format=raw,file=./bin/os.bin \
		-qmp unix:./qmp-sock,server,wait=off -d guest_errors,cpu_reset #-s -S 

endif

ifeq ($(UNAME_S),Darwin)
main: clean bootloader kernel run

bootloader:
	mkdir -p bin
	nasm -f bin src/boot/boot.asm -o bin/bootloader.bin

kernel:
	x86_64-elf-gcc $(CFLAGS) -m32 -c src/kernel/kernel.c -o ./bin/kernel.c.o
	x86_64-elf-gcc $(CFLAGS) -m32 -c src/kernel/vga.c -o ./bin/vga.c.o
	x86_64-elf-gcc $(CFLAGS) -m32 -c src/kernel/idt.c -o ./bin/idt.c.o
	x86_64-elf-gcc $(CFLAGS) -m32 -c src/kernel/isr.c -o ./bin/isr.c.o

	nasm -f elf src/kernel/kernelstrap.asm -o ./bin/kernelstrap.asm.o

	# First, link with symbols into ELF (for debugging)
	x86_64-elf-ld -m elf_i386 -o ./bin/kernel.elf -Ttext 0x1000 \
		./bin/kernelstrap.asm.o \
		./bin/kernel.c.o \
		./bin/vga.c.o \
		./bin/idt.c.o \
		./bin/isr.c.o

	# Then, make the flat binary (for booting)
	x86_64-elf-ld -m elf_i386 -o ./bin/kernel.bin -Ttext 0x1000 \
		./bin/kernelstrap.asm.o \
		./bin/kernel.c.o \
		./bin/vga.c.o \
		./bin/idt.c.o \
		./bin/isr.c.o \
		--oformat binary

objdump:
	x86_64-elf-objdump -d ./bin/kernel.elf > ./bin/kernel.asm

run:
	cat ./bin/bootloader.bin ./bin/kernel.bin > ./bin/os.bin
	qemu-system-x86_64 -drive format=raw,file=./bin/os.bin \
		-qmp unix:./qmp-sock,server,wait=off \
		-d guest_errors,cpu_reset

clean:
	rm -rf ./bin
endif
