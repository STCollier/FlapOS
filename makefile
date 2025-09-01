UNAME_S := $(shell uname -s)
CC=i386-elf-gcc
LD=i386-elf-ld
OUT=./bin
_OBJS=
ifeq ($(UNAME_S),Linux)
CFLAGS = -ffreestanding -I/usr/lib/gcc/i386-elf/15.1.0/include/ -g -Wall -Wextra -Wno-unused-function -Wno-unused-variable -Wno-unused-parameter\
         -fno-exceptions -nostdlib -nostdinc -fno-stack-protector \
         -fno-builtin-function -fno-builtin \
		 -mno-sse -mno-sse2 -mfpmath=387 -march=i386
endif
ifeq ($(UNAME_S),Darwin)
CFLAGS = -ffreestanding -I/usr/lib/gcc/i386-elf/15.1.0/include/ -g -Wall -Wextra \
         -fno-exceptions -m32 -nostdlib -nostdinc -fno-stack-protector \
         -fno-builtin-function -fno-builtin \
		 -mno-sse -mno-sse2 -mfpmath=387 -march=i386
CC=x86_64-elf-gcc
LD=x86_64-elf-ld
endif
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
		-qmp unix:./qmp-sock,server,wait=off \
		-d guest_errors,cpu_reset
