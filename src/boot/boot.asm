[org 0x7c00]

.start:
    mov byte [DL_AT_BOOT], dl

    xor ax, ax
    mov ds, ax
    mov ss, ax
    mov fs, ax
    mov gs, ax
    mov es, ax
    ; reset all our segmentation registers. idk if morbords actually leave them dirty but im cleaning it like a wagie who doesnt trust his coworkers but the CEO lowk is coming tmrw
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

    lgdt [gdt_descriptor] ; load gdt (the below implementation i found used EVERYWHERE)

    ;lastly we will need to enable a20 to get to PM.
    in al, 0x92
    or al, 2
    out 0x92, al

   
    cli
    mov eax, cr0
    or al, 1 ; i would just do eax but osdev said to do al so im doing al
    mov cr0, eax
    ; we are now in protected mode. we now will enter the kernel. the kernel is loaded at the GDT entry base address. we can use the GDT by performing a far jump.
    ; the far jump syntax is as follows: `jmp GDTBYTE:OFFSET` where GDTBYTE is the start of the gdt entry you are jumping to and offset is added onto that base address.
    ; we want to jump to the code segment (second entry including null desc) so the first value is 08h, or 8 bytes into the gdt. the second value will be 0 and we will strap on an instruction at the beginning of the file to jump to the kernel main.
    
    ;jmp 08h:0
    jmp 0x08:KERNEL_POSITION 
    ; in cfellnosas tutorial CODE_SEG and DATA_SEG are set to their respective gdt offsets, but thats stupid and i can remember 0x8 and 0x10.
    ; furthermore, none of this code will ever be used for anything except the jump so our GDT is already temporary and shouldnt be reused, thus i wont need to remember it in the future.
    [bits 32]
    ; kernal takes over, do we really even need [bits 32]? idk

; jdh uses this exact same implemtation and some other sources so it has to be good
gdt_start: ; don't remove the labels, they're needed to compute sizes and jumps
    ; the GDT starts with a null 8-byte
    dd 0x0 ; 4 byte
    dd 0x0 ; 4 byte

; GDT for code segment. base = 0x00000000, length = 0xfffff
; for flags, refer to os-dev.pdf document, page 36
gdt_code: 
    dw 0xffff    ; segment length, bits 0-15
    dw 0x0       ; segment base, bits 0-15
    db 0x0       ; segment base, bits 16-23
    db 10011010b ; flags (8 bits)
    db 11001111b ; flags (4 bits) + segment length, bits 16-19
    db 0x0       ; segment base, bits 24-31

; GDT for data segment. base and length identical to code segment
; some flags changed, again, refer to os-dev.pdf
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
KERNEL_SIZE     equ 3

times 510-($-$$) db 0
dw 0xAA55