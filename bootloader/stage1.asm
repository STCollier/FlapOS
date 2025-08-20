[org 7c00h] ; "origin", bios jumps to 7c00 in memory and executes the first sector if magic bytes are present 512 bytes ahead.
;Begin constants
DISK_LOAD_DL equ 80h ; specifies first disk, e0h means first CD/DVD, 0h is first floppy disk
KERNEL_POSITION equ 0x1000
;End constants
xor cs, cs
mov ds, cs
mov ss, cs
mov es, cs
mov fs, cs
mov gs, cs
; reset all our segmentation registers. some morbords will do this but some leave them dirty.

; we can get straight to loading the kernel at this point.
; this is programmed to run on a disk, so in qemu we will need to put our code on hard disk
; parameters for int 13h:
;   ah=command (02h is the one we look for)
;   al=sector count
;   ch=cylinder
;   cl=sector
;   dh=head
;   dl=drive
;   the output is at es:bx.
mov ah, 2 ; read
xor ch, ch
mov cl, 2 ; sector 2 is the start sector (the one after bootsector)
xor dh, dh
mov dl, DISK_LOAD_DL ; select the drive we want to read from
mov bx, KERNEL_POSITION ; destination address
int 13h

;setting up GDT
cli
;first clear interrupts, can cause unexpected behavior if they are tripped

;the GDTR uses LINEAR memory space, NOT physical. this means we have to account for our data seg. it SHOULD be zero, but if the preceding code is changed it might end up as a different value and thus this code will account for segmentation.
xor eax, eax ; clear eax
mov ax, ds
shl eax,4  ; load in data segment and shift it to be in the correct position (i know its crazy but shifting 4 bits is actually correct)
add eax, GDT_START ; add in our offset, and with this we have our gdtr offset set up in eax.
mov [GDTR_START+2], eax



[section .data]
;Begin data
;Begin GDT
GDTR_START:
GDTR_SIZE: dw 0
GDTR_OFFSET: dd 0

GDTR_END:
GDT_START:
GDT_END:
;End GDT
;End data



;LINKS:
;https://wiki.osdev.org/Global_Descriptor_Table
;https://wiki.osdev.org/GDT_Tutorial#Basics
;https://en.wikipedia.org/wiki/INT_13H
;https://wiki.osdev.org/Segmentation