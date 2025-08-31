#include "isr.h"
#include "util.h"
#include "vga.h"
#include "idt.h"
#include "timer.h"
#include "keyboard.h"

isr_t interrupt_handlers[256];

void isr_init() {
    set_idt_gate(0,  (uintptr_t) ISR0);
    set_idt_gate(1,  (uintptr_t) ISR1);
    set_idt_gate(2,  (uintptr_t) ISR2);
    set_idt_gate(3,  (uintptr_t) ISR3);
    set_idt_gate(4,  (uintptr_t) ISR4);
    set_idt_gate(5,  (uintptr_t) ISR5);
    set_idt_gate(6,  (uintptr_t) ISR6);
    set_idt_gate(7,  (uintptr_t) ISR7);
    set_idt_gate(8,  (uintptr_t) ISR8);
    set_idt_gate(9,  (uintptr_t) ISR9);
    set_idt_gate(10, (uintptr_t) ISR10);
    set_idt_gate(11, (uintptr_t) ISR11);
    set_idt_gate(12, (uintptr_t) ISR12);
    set_idt_gate(13, (uintptr_t) ISR13);
    set_idt_gate(14, (uintptr_t) ISR14);
    set_idt_gate(15, (uintptr_t) ISR15);
    set_idt_gate(16, (uintptr_t) ISR16);
    set_idt_gate(17, (uintptr_t) ISR17);
    set_idt_gate(18, (uintptr_t) ISR18);
    set_idt_gate(19, (uintptr_t) ISR19);
    set_idt_gate(20, (uintptr_t) ISR20);
    set_idt_gate(21, (uintptr_t) ISR21);
    set_idt_gate(22, (uintptr_t) ISR22);
    set_idt_gate(23, (uintptr_t) ISR23);
    set_idt_gate(24, (uintptr_t) ISR24);
    set_idt_gate(25, (uintptr_t) ISR25);
    set_idt_gate(26, (uintptr_t) ISR26);
    set_idt_gate(27, (uintptr_t) ISR27);
    set_idt_gate(28, (uintptr_t) ISR28);
    set_idt_gate(29, (uintptr_t) ISR29);
    set_idt_gate(30, (uintptr_t) ISR30);
    set_idt_gate(31, (uintptr_t) ISR31);

    // Remap the PIC
    outportb(0x20, 0x11);
    outportb(0xA0, 0x11);
    outportb(0x21, 0x20);
    outportb(0xA1, 0x28);
    outportb(0x21, 0x04);
    outportb(0xA1, 0x02);
    outportb(0x21, 0x01);
    outportb(0xA1, 0x01);
    outportb(0x21, 0x0);
    outportb(0xA1, 0x0);

    // Install the IRQs
    set_idt_gate(32, (uintptr_t) _IRQ0);
    set_idt_gate(33, (uintptr_t) _IRQ1);
    set_idt_gate(34, (uintptr_t) _IRQ2);
    set_idt_gate(35, (uintptr_t) _IRQ3);
    set_idt_gate(36, (uintptr_t) _IRQ4); 
    set_idt_gate(37, (uintptr_t) _IRQ5);
    set_idt_gate(38, (uintptr_t) _IRQ6);
    set_idt_gate(39, (uintptr_t) _IRQ7);
    set_idt_gate(40, (uintptr_t) _IRQ8);
    set_idt_gate(41, (uintptr_t) _IRQ9);
    set_idt_gate(42, (uintptr_t) _IRQ10);
    set_idt_gate(43, (uintptr_t) _IRQ11);
    set_idt_gate(44, (uintptr_t) _IRQ12);
    set_idt_gate(45, (uintptr_t) _IRQ13);
    set_idt_gate(46, (uintptr_t) _IRQ14);
    set_idt_gate(47, (uintptr_t) _IRQ15);

    set_idt();
}  

static bool HANG = false;
void isr_handler(registers_t r) {
    klog("Exception %x (%s) at EIP=%x CS=%x EFLAGS=%x", r.int_no, EXCEPTIONS[r.int_no], r.eip, r.cs, r.eflags);

    if (HANG) for(;;);
}


void register_interrupt_handler(uint8_t n, isr_t handler) {
    interrupt_handlers[n] = handler;
}

void irq_handler(registers_t r) {
    /* After every interrupt we need to send an EOI to the PICs
     * or they will not send another interrupt again */
    if (r.int_no >= 40) outportb(0xA0, 0x20); /* slave */
    outportb(0x20, 0x20); /* master */

    /* Handle the interrupt in a more modular way */
    if (interrupt_handlers[r.int_no] != 0) {
        isr_t handler = interrupt_handlers[r.int_no];
        handler(r);
    }
}

void irq_init() {
    asm volatile("sti");
    timer_init(60); // 60 TPS (or FPS depending on how you look at it)
    keyboard_init();
}