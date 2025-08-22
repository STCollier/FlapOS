UNAME_S := $(shell uname -s)
CC=i386-elf-gcc
LD=i386-elf-ld

ifeq ($(UNAME_S),Linux)
# you do you
# LOL ok
CFLAGS= -ffreestanding -g -Wall -Wextra -fno-exceptions -m32 -nostdlib -nostdinc -fno-stack-protector fno-builtin-function -fno-builtin

main: start bootloader kernel concat qemu
start:
	rm -rf ./bin
	mkdir -p ./bin
bootloader:
	nasm -f bin src/boot/boot.asm -o bin/bootloader.bin
kernel:
	$(CC) $(CFLAGS) -c src/kernel/kernel.c -o ./bin/kernel.c.o
	$(CC) $(CFLAGS) -c src/kernel/font.c -o ./bin/font.c.o
	$(CC) $(CFLAGS) -c src/kernel/graphics.c -o ./bin/graphics.c.o

	nasm -f elf src/kernel/kernelstrap.asm -o ./bin/kernelstrap.asm.o
	$(LD) --oformat binary -o ./bin/kernel.bin -Ttext 0x1000 ./bin/kernelstrap.asm.o \
		./bin/kernel.c.o   \
		./bin/font.c.o     \
		./bin/graphics.c.o \
concat:
	cat ./bin/bootloader.bin ./bin/kernel.bin > ./bin/os.bin
qemu:
	qemu-system-x86_64 -drive format=raw,file=./bin/os.bin  -qmp unix:./qmp-sock,server,wait=off -d guest_errors,cpu_reset #-s -S 

endif

ifeq ($(UNAME_S),Darwin)
main: clean bootloader kernel run

bootloader:
	mkdir -p bin
	nasm -f bin src/boot/boot.asm -o bin/bootloader.bin

kernel:
	x86_64-elf-gcc $(CFLAGS) -m32 -c src/kernel/kernel.c -o ./bin/kernel.c.o
	x86_64-elf-gcc $(CFLAGS) -m32 -c src/kernel/font.c -o ./bin/font.c.o
	x86_64-elf-gcc $(CFLAGS) -m32 -c src/kernel/graphics.c -o ./bin/graphics.c.o

	nasm -f elf src/kernel/kernelstrap.asm -o ./bin/kernelstrap.asm.o
	x86_64-elf-ld -m elf_i386 -o ./bin/kernel.bin -Ttext 0x1000 ./bin/kernelstrap.asm.o \
		./bin/kernel.c.o   \
		./bin/font.c.o     \
		./bin/graphics.c.o \
		--oformat binary

run:
	cat ./bin/bootloader.bin ./bin/kernel.bin > ./bin/os.bin
	qemu-system-x86_64 -drive format=raw,file=./bin/os.bin  -qmp unix:./qmp-sock,server,wait=off -d guest_errors,cpu_reset
	

clean:
	rm -rf ./bin
endif