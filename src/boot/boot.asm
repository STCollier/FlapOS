[org 0x7c00]

.start:
    cli

    xor ax, ax
    mov ds, ax
    mov ss, ax
    mov fs, ax
    mov gs, ax
    mov es, ax

    mov [DL_AT_BOOT], dl
    xor dh, dh 
    mov bx, KERNEL_POSITION 
    mov cl, 2
    load:
    mov ah, 2               ; read command
    mov al, 1               ; sector amnt
    xor ch, ch              ; cylinder pos
    ; sector # is set already
    ; head num set already
  mov [DL_AT_BOOT], dl

    ; begin 13h
    mov ah, 2               ; read command
    mov al, KERNEL_SIZE     ; kernel sector count
    xor ch, ch              ; cylinder pos
    mov cl, 2               ; sector 2 is the start sector (the one after bootsector)
    xor dh, dh              ; head number
    mov dl, [DL_AT_BOOT]
    
    mov bx, KERNEL_POSITION ; destination address
    int 13h 
    ; set video mode 13h (320x200x256)
    mov ah, 0x00
    mov al, 0x13
    int 0x10

    ; load GDT
    lgdt [gdt_descriptor]
    
    ; enable a20 to get to PM.
    in al, 0x92
    or al, 2
    out 0x92, al

    
    mov eax, cr0
    or eax, 1
    mov cr0, eax

    jmp 0x08:entry32

[bits 32]
entry32:
    ;set other segment registers to the data seg
    mov ax, 10h
    mov ds, ax
    mov ss, ax
    mov fs, ax
    mov gs, ax
    mov es, ax

    mov esp, STACK_END 
    jmp 0x08:KERNEL_POSITION

    
gdt_start:
    ;gdt null sec
    dq 0

; code seg desc. base = 0x00000000, length = 0xfffff
gdt_code: 
    dw 0xffff    ; segment length bits 0-15
    dw 0x0       ; segment base bits 0-15
    db 0x0       ; segment base bits 16-23
    db 10011010b ; access byte
    db 11001111b ; flags (4 bits) + segment length bits 16-19
    db 0x0       ; segment base bits 24-31
gdt_data:
    dw 0xffff
    dw 0x0
    db 0x0
    db 10010010b
    db 11001111b
    db 0x0

gdt_end:

; GDT descriptor
gdt_descriptor:
    dw gdt_end - gdt_start - 1 ; size (16 bit), always one less of its true size
    dd gdt_start ; address (32 bit)
    

DL_AT_BOOT:     db 0
KERNEL_POSITION equ 0x1000
KERNEL_SIZE     equ 63
STACK_END:      equ 0x10000

times 510-($-$$) db 0
dw 0xAA55
