UNAME_S := $(shell uname -s)
CC=
LD=
OUT=./bin
NO_SSE = -mno-sse -mno-sse2 -mfpmath=387 -march=i386
ifeq ($(UNAME_S),Linux)
CFLAGS = -ffreestanding -I/usr/lib/gcc/i386-elf/15.1.0/include/ -Wall -Wextra -Wno-unused-function -Wno-unused-variable \
         -fno-exceptions -nostdlib -nostdinc -fno-stack-protector \
         -fno-builtin-function -fno-builtin 
CC=i386-elf-gcc -m32
LD=i386-elf-ld
endif 
ifeq ($(UNAME_S),Darwin)
CFLAGS = -ffreestanding -I/opt/homebrew/Cellar/gcc/14.2.0_1/lib/gcc/current/gcc/aarch64-apple-darwin24/14/include -g -Wall -Wextra \
         -fno-exceptions -nostdlib -nostdinc -fno-stack-protector \
         -fno-builtin-function -fno-builtin \
		 -Wno-unused-function -Wno-unused-variable
CC=x86_64-elf-gcc -m32
LD=x86_64-elf-ld -melf_i386
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
	@nasm -f elf src/kernel/kernel.asm -o ./bin/kernel/kernel.asm.o
	@elfs="";\
	for file in src/kernel/*.c;\
	do \
	    echo "Build $$file"; \
	    $(CC) -c $(NO_SSE) $(CFLAGS) $$file -o $(OUT)/kernel/$$(basename $${file}.o) ;\
	    elfs+="$(OUT)/kernel/$$(basename $${file}.o) "; \
	done ;  \
	$(LD) -o $(OUT)/kernel.bin --oformat binary -T linker.ld $(OUT)/kernel/kernel.asm.o $$elfs
os:
	@echo "Build final OS"
	@cat $(OUT)/boot.bin $(OUT)/kernel.bin > $(OUT)/os.bin
	@echo "Full image at $(OUT)/os.bin"
qemu:
	qemu-system-i386 -drive format=raw,file=$(OUT)/os.bin \
	    -qmp unix:./qmp-sock,server,wait=off 