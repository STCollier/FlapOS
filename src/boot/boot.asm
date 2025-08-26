[org 0x7c00]

.start:
    mov byte [DL_AT_BOOT], dl

    xor ax, ax
    mov ds, ax
    mov ss, ax
    mov fs, ax
    mov gs, ax
    mov es, ax
    ; reset seg registers
    ;begin 13h
    cli
    mov ah, 2 ; read command
    mov al, KERNEL_SIZE ; kernel sector count
    xor ch, ch ; cylinder pos
    mov cl, 2 ; sector 2 is the start sector (the one after bootsector)
    xor dh, dh ; head number
    mov dl, [DL_AT_BOOT]
    mov bx, KERNEL_POSITION ; destination address
    int 13h

    ; set video mode 13h (320x200x256) for later
    mov ah, 0x00
    mov al, 0x13
    int 0x10
    cli
    lgdt [gdt_descriptor] ; load gdt (the below implementation i found used EVERYWHERE)
    
    
    
    ;lastly we will need to enable a20 to get to PM.
    in al, 0x92
    or al, 2
    out 0x92, al

   
    
    mov eax, cr0
    or al, 1 ; i would just do eax but osdev said to do al so im doing al
    mov cr0, eax
    ; we are now in protected mode, do some init and hand over to kernel
    
    ;jmp 08h:0
    ;[bits 32]
    mov esp, STACK_END ; stack grows backwards
    
    
    ;set other segment registers to the data seg
    mov ax, 10h
    mov ds, ax
    mov ss, ax
    mov fs, ax
    mov gs, ax
    mov es, ax
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