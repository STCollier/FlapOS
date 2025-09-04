global _start

[extern kmain]
[extern isr_handler]
[extern irq_handler]

[bits 32]
_start:
    call kmain
    jmp $

%include "src/interrupts/idt.asm"