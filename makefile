UNAME_S := $(shell uname -s)
CC=
LD=
OUT=./bin
ifeq ($(UNAME_S),Linux)
CFLAGS = -ffreestanding -I/usr/lib/gcc/i386-elf/15.1.0/include/ -Wall -Wextra -Wno-unused-function -Wno-unused-variable -Wno-unused-parameter\
         -fno-exceptions -nostdlib -nostdinc -fno-stack-protector \
         -fno-builtin-function -fno-builtin 
CC=i386-elf-gcc
LD=i386-elf-ld

all: clean bootloader kernel os qemu
clean:
	@echo "Clean $(OUT)"
	@rm -rf $(OUT)
	@mkdir -p $(OUT)
bootloader:
	@echo "Build Bootloader"
	@nasm -f bin src/boot/boot.asm -o $(OUT)/boot.bin
kernel:
	@echo "Build kernel"
	@mkdir -p $(OUT)/kernel
	@nasm -f elf src/kernel/kernelstrap.asm -o ./bin/kernel/kernelstrap.asm.o
	@elfs="";\
	for file in src/kernel/*.c;\
	do \
		echo "Build $$file"; \
		$(CC) -c $(CFLAGS) $$file -o $(OUT)/kernel/$$(basename $${file}.o) ;\
		elfs+="$(OUT)/kernel/$$(basename $${file}.o) "; \
	done ;  \
	$(LD) -o $(OUT)/kernel.bin --oformat binary -Ttext 0x1000 $(OUT)/kernel/kernelstrap.asm.o $$elfs
os:
	@echo "Build final OS"
	@cat $(OUT)/boot.bin $(OUT)/kernel.bin > $(OUT)/os.bin
	@echo "Full image at $(OUT)/os.bin"
qemu:
	qemu-system-i386 -drive format=raw,file=$(OUT)/os.bin \
		-qmp unix:./qmp-sock,server,wait=off  

endif


# sorry bro I couldn't get your makefile to work imma ball with mine


ifeq ($(UNAME_S),Darwin)

NO_SSE = -mno-sse -mno-sse2 -mfpmath=387 -march=i386

CFLAGS = -ffreestanding -I/opt/homebrew/Cellar/gcc/14.2.0_1/lib/gcc/current/gcc/aarch64-apple-darwin24/14/include -g -Wall -Wextra \
         -fno-exceptions -m32 -nostdlib -nostdinc -fno-stack-protector \
         -fno-builtin-function -fno-builtin

main: clean bootloader kernel run

bootloader:
	mkdir -p bin
	nasm -f bin src/boot/boot.asm -o bin/bootloader.bin

kernel:
	x86_64-elf-gcc $(CFLAGS) -m32 -c src/kernel/kernel.c -o ./bin/kernel.c.o
	x86_64-elf-gcc $(CFLAGS) -m32 -c src/kernel/vga.c -o ./bin/vga.c.o
	x86_64-elf-gcc $(CFLAGS) -m32 -c src/kernel/idt.c -o ./bin/idt.c.o 			 $(NO_SSE)
	x86_64-elf-gcc $(CFLAGS) -m32 -c src/kernel/isr.c -o ./bin/isr.c.o 			 $(NO_SSE)
	x86_64-elf-gcc $(CFLAGS) -m32 -c src/kernel/timer.c -o ./bin/timer.c.o	     $(NO_SSE)
	x86_64-elf-gcc $(CFLAGS) -m32 -c src/kernel/keyboard.c -o ./bin/keyboard.c.o $(NO_SSE)
	x86_64-elf-gcc $(CFLAGS) -m32 -c src/kernel/bird.c -o ./bin/bird.c.o $(NO_SSE)
	x86_64-elf-gcc $(CFLAGS) -m32 -c src/kernel/pipes.c -o ./bin/pipes.c.o $(NO_SSE)

	nasm -f elf src/kernel/kernelstrap.asm -o ./bin/kernelstrap.asm.o

	# Then, make the flat binary (for booting)
	x86_64-elf-ld -m elf_i386 -Tsrc/kernel/link.ld -o ./bin/kernel.bin \
		./bin/kernelstrap.asm.o \
		./bin/kernel.c.o \
		./bin/vga.c.o \
		./bin/idt.c.o \
		./bin/isr.c.o \
		./bin/timer.c.o \
		./bin/keyboard.c.o \
		./bin/bird.c.o \
		./bin/pipes.c.o \
		--oformat binary

objdump:
	x86_64-elf-objdump -d ./bin/kernel.elf > ./bin/kernel.asm

run:
	cat ./bin/bootloader.bin ./bin/kernel.bin > ./bin/os.bin
	qemu-system-x86_64 -drive format=raw,file=./bin/os.bin \
		-qmp unix:./qmp-sock,server,wait=off \
		-d guest_errors,cpu_reset \


clean:
	rm -rf ./bin
endif
