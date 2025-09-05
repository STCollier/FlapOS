#include "isr.h"
#include "../util.h"
#include "../drivers/vga.h"
#include "idt.h"
#include "../drivers/timer.h"
#include "../drivers/keyboard.h"

#define SCAN(...) __VA_ARGS__

#define GEN_(_offset, _pfx, _i) set_idt_gate(_i + _offset, (uintptr_t) _pfx##_i)
#define GEN(_offset, _pfx, ...) GEN_(_offset, _pfx, __VA_ARGS__) GEN2(_offset, _pfx,
#define GEN2(_offset, _pfx, ...) GEN2_##__VA_OPT__(1)(_offset, _pfx, __VA_ARGS__)
#define GEN2_(...) ;
#define GEN2_1(_offset, _pfx, ...) ; GEN_(_offset, _pfx, __VA_ARGS__) GEN3(_offset, _pfx,
#define GEN3(_offset, _pfx, ...) GEN3_##__VA_OPT__(1)(_offset, _pfx, __VA_ARGS__)
#define GEN3_(...) ;
#define GEN3_1(_offset, _pfx, ...) ; GEN_(_offset, _pfx, __VA_ARGS__) GEN2(_offset, _pfx,

#define SEQ_ISR(_offset, _pfx) (_offset, _pfx, 0)1)2)3)4)5)6)7)8)9)10)11)12)13)14)15)16)17)18)19)20)21)22)23)24)25)26)27)28)29)30)31))
#define SEQ_IRQ(_offset, _pfx) (_offset, _pfx, 0)1)2)3)4)5)6)7)8)9)10)11)12)13)14)15))

#define ISRS_INSTALL() SCAN(GEN SEQ_ISR(0, ISR))
#define IRQS_INSTALL() SCAN(GEN SEQ_IRQ(32, _IRQ))

isr_t interrupt_handlers[256];

void isr_init() {
    ISRS_INSTALL();

    // Remap PIC
    outportb(PIC1, ICW1_INIT | ICW1_ICW4);
    outportb(PIC2, ICW1_INIT | ICW1_ICW4);
    outportb(PIC1_DATA, PIC1_OFFSET);
    outportb(PIC2_DATA, PIC2_OFFSET);
    outportb(PIC1_DATA, 0x04);
    outportb(PIC2_DATA, 0x02);
    outportb(PIC1_DATA, PIC_MODE_8086);
    outportb(PIC1_DATA, PIC_MODE_8086);
    outportb(PIC1_DATA, 0x0);
    outportb(PIC2_DATA, 0x0);

    IRQS_INSTALL();

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
    if (r.int_no >= 40) outportb(PIC2, PIC_EOI); // slave
    outportb(PIC1, PIC_EOI); // master

    if (interrupt_handlers[r.int_no] != 0) {
        isr_t handler = interrupt_handlers[r.int_no];
        handler(r);
    }
}

void irq_init() {
    asm volatile("sti");
    timer_init(TIMER_HZ);
    keyboard_init();
}