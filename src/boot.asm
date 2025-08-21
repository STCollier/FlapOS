[org 0x7c00]

.start:
    mov si, 0 ; x position counter
    mov bx, 0 ; color counter

    ; set video mode 13h (320x200x256)
    mov ah, 0x00
    mov al, 0x13
    int 0x10

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
    jmp $

rainbow db 4,6,14,2,9,5
rainbow_len equ $-rainbow

times 510-($-$$) db 0
dw 0xAA55
