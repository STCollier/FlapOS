; this doesnt need to be compiled individually, it is compiled with the kernel strap

; ISR with no error
%macro ISR_NO_ERR 1
    global ISR%1
ISR%1:
    cli
    push dword 0 
    push dword %1
    jmp isr_common
%endmacro

; ISR with error code
%macro ISR_ERR 1
    global ISR%1
ISR%1:
    cli
    push dword %1
    jmp isr_common
%endmacro

; IRQ [32, 47]
%macro IRQ 1
    global _IRQ%1
_IRQ%1:
    cli
    push dword %1
    push dword (32 + %1)
    jmp irq_common
%endmacro

ISR_NO_ERR 0
ISR_NO_ERR 1
ISR_NO_ERR 2
ISR_NO_ERR 3
ISR_NO_ERR 4
ISR_NO_ERR 5
ISR_NO_ERR 6
ISR_NO_ERR 7
ISR_ERR 8
ISR_NO_ERR 9
ISR_ERR 10
ISR_ERR 11
ISR_ERR 12
ISR_ERR 13
ISR_ERR 14
ISR_NO_ERR 15
ISR_NO_ERR 16
ISR_NO_ERR 17
ISR_NO_ERR 18
ISR_NO_ERR 19
ISR_NO_ERR 20
ISR_NO_ERR 21
ISR_NO_ERR 22
ISR_NO_ERR 23
ISR_NO_ERR 24
ISR_NO_ERR 25
ISR_NO_ERR 26
ISR_NO_ERR 27
ISR_NO_ERR 28
ISR_NO_ERR 29
ISR_NO_ERR 30
ISR_NO_ERR 31

IRQ 0  ; 32
IRQ 1
IRQ 2
IRQ 3
IRQ 4
IRQ 5
IRQ 6
IRQ 7
IRQ 8
IRQ 9
IRQ 10
IRQ 11
IRQ 12
IRQ 13
IRQ 14
IRQ 15 ; 45

; Common ISR code
isr_common:
	pusha 
	mov ax, ds
	push eax
	mov ax, 0x10 
	mov ds, ax
	mov es, ax
	mov fs, ax
	mov gs, ax
	

	call isr_handler
	
	pop eax 
	mov ds, ax
	mov es, ax
	mov fs, ax
	mov gs, ax
	popa
	add esp, 8
	sti
	iret

irq_common:
    pusha 
    mov ax, ds
    push eax
    mov ax, 0x10
    mov ds, ax
    mov es, ax
    mov fs, ax
    mov gs, ax
    call irq_handler ; different than ISR 
    pop ebx          ; different than ISR
    mov ds, bx
    mov es, bx
    mov fs, bx
    mov gs, bx
    popa
    add esp, 8
    sti
    iret 