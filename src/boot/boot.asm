[org 0x7c00]

%include "src/const.asm"

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
    ; carry flag is set if error, handle later
    ;at this point the kernel should be loaded in es:bx. we just make a jump after setting up gdt and 32 bit mode
    ;setting up GDT
    cli ;first clear interrupts, can cause unexpected behavior if they are tripped
    ;the GDTR uses LINEAR memory space, NOT physical. this means we have to account for our data seg. it SHOULD be zero, but if the preceding code is changed it might end up as a different value and thus this code will account for segmentation.
    xor eax, eax 
    mov ax, ds
    shl eax,4  ; load in data segment and shift it to be in the correct position (i know its crazy but shifting 4 bits is actually correct)
    add eax, GDT_START ; add in our offset, and with this we have our gdtr offset set up in eax.
    mov [GDTR_START+2], eax
    mov eax, GDT_END
    sub eax, GDT_START
    sub eax, 1 ; limit - 1  
    mov [GDTR_START], ax
    lgdt [GDTR_START]

    ;lastly we will need to enable a20 to get to PM.
    in al, 0x92
    or al, 2
    out 0x92, al


    mov si, 0 ; x position counter
    mov bx, 0 ; color counter

    ; set video mode 13h (320x200x256) for later
    mov ah, 0x00
    mov al, 0x13
    int 0x10
    ; and now, DRAWING!!!
    .draw:
    cmp si, 320
    je .done

    mov al, [rainbow + bx]
    mov ah, 0x0C
    mov bh, 0x00
    mov cx, si
    mov dx, 15
    int 0x10

    inc si
    inc bx
    cmp bx, rainbow_len
    jne .draw
    mov bx, 0
    jmp .draw
    .done:
    ; now that we have randomly started drawing lines lets get PM set up.
    mov eax, cr0
    or al, 1 ; i would just do eax but osdev said to do al so im doing al
    mov cr0, eax
    ; we are now in protected mode. we now will enter the kernel. the kernel is loaded at the GDT entry base address. we can use the GDT by performing a far jump.
    ; the far jump syntax is as follows: `jmp GDTBYTE:OFFSET` where GDTBYTE is the start of the gdt entry you are jumping to and offset is added onto that base address.
    ; we want to jump to the code segment (second entry including null desc) so the first value is 08h, or 8 bytes into the gdt. the second value will be 0 and we will strap on an instruction at the beginning of the file to jump to the kernel main.
    [bits 32]
    
    ;jmp 08h:0
    call KERNEL_POSITION
    ; in cfellnosas tutorial CODE_SEG and DATA_SEG are set to their respective gdt offsets, but thats stupid and i can remember 0x8 and 0x10.
    ; furthermore, none of this code will ever be used for anything except the jump so our GDT is already temporary and shouldnt be reused, thus i wont need to remember it in the future.
    
    jmp $ 
.data:
;Begin GDT
GDTR_START:
GDTR_SIZE: dw 0
GDTR_OFFSET: dd 0

GDTR_END:
GDT_START:
GDT_NULLDESC: dq 0 ; each descriptor 8 bytes, first one is null
GDT_ENT_1:
    GDT_ENT_1_LIMIT0: dw 0xffff ; Descriptor length bits 0-15
    GDT_ENT_1_BASE0: dw KERNEL_POSITION ; Descriptor linear base address bits 0-15. the base address is 0x1000
    GDT_ENT_1_BASE1: db 0
    GDT_ENT_1_ACCESSBYTE: db  0b10011100 ; https://wiki.osdev.org/Global_Descriptor_Table, read access byte to know what each flag does
    GDT_ENT_1_LIMIT1_AND_FLAGS: db 0b01000000 ;flags first, then 0s for limit. that gives us 4k to work with in our code segment
    GDT_ENT_1_BASE2: dw 0
    
    GDT_ENT_2_LIMIT0: dw 0xffff 
    GDT_ENT_2_BASE0: dw 0x1000 
    GDT_ENT_2_BASE1: db 1 ;code base address is 0x1000 plus 0xffff limit = 0x11000 base address for next segment
    GDT_ENT_2_ACCESSBYTE: db  0b10010010 
    GDT_ENT_2_LIMIT1_AND_FLAGS: db 0b01000000 ;flags first, then 0s for limit. that gives us 4k to work with in data seg as well, which we can extend by enabling G flag or adding to limit.
    GDT_ENT_2_BASE2: dw 0
    
GDT_END:
DL_AT_BOOT: db 0
;End GDT
rainbow db 4,6,14,2,9,5
rainbow_len equ $-rainbow

times 510-($-$$) db 0
dw 0xAA55