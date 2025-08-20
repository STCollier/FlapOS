UNAME_S := $(shell uname -s)

ifeq ($(UNAME_S),Linux)
# you do you
main:
qemu:
endif

ifeq ($(UNAME_S),Darwin)
boot:
	# putting everything in the initial make target allows you just to run `make` to compile and emulate
	mkdir -p ./bin
	nasm -f bin src/boot.asm -o bin/boot.bin
	qemu-system-x86_64 -drive format=raw,file=bin/boot.bin

clean:
	rm -rf ./bin
endif