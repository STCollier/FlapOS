global _start
extern isr_handle
global ISR_TABLE
[extern kmain]
_start:

[bits 32]

call kmain
jmp $
_knl_loop:
    hlt
    jmp _knl_loop
isr_stub:
    pusha
    mov ax, ds
    push eax
    mov ax,0x10
    mov ds, ax
	mov es, ax
	mov fs, ax
	mov gs, ax

    call isr_handle

    pop eax
    mov ds, ax
    mov es, ax
	mov fs, ax
	mov gs, ax
    popa
    add esp,0x8 ; remove uin32_t errorcode, uint32_t isrcode
    sti ;isr disables interrupts
    iret
%macro isr_nocode 1
global isr%+%1
isr%+%1:
    cli
    push byte -1
    push byte %1
    jmp isr_stub
%endmacro
%macro isr_code 1
global isr%+%1
isr%+%1:
    cli
    push byte %1
    jmp isr_stub
%endmacro
isr_nocode 0
isr_nocode 1
isr_nocode 2
isr_nocode 3
isr_nocode 4
isr_nocode 5
isr_nocode 6
isr_nocode 7
isr_code 8
isr_nocode 9
isr_code 10
isr_code 11
isr_code 12
isr_code 13
isr_code 14
isr_nocode 15
isr_nocode 16
isr_code 17
isr_nocode 18
isr_nocode 19
isr_nocode 20
isr_code 21
isr_nocode 22
isr_nocode 23
isr_nocode 24
isr_nocode 25
isr_nocode 26
isr_nocode 27
isr_nocode 28
isr_nocode 29
isr_nocode 30
isr_nocode 31

ISR_TABLE:
dd isr0
dd isr1
dd isr2
dd isr3
dd isr4
dd isr5
dd isr6
dd isr7
dd isr8
dd isr9
dd isr10
dd isr11
dd isr12
dd isr13
dd isr14
dd isr15
dd isr16
dd isr17
dd isr18
dd isr19
dd isr20
dd isr21
dd isr22
dd isr23
dd isr24
dd isr25
dd isr26
dd isr27
dd isr28
dd isr29
dd isr30
dd isr31