#include "vga.h"
#include "util.h"
#include "idt.h"

void idt_set_descriptor(uint8_t vector, void* isr, uint8_t flags) {
    idt_entry_t* descriptor = &idt[vector];

    descriptor->isr_low        = (uint32_t)isr & 0xFFFF;
    descriptor->kernel_cs      = 0x08; // this value can be whatever offset your kernel code selector is in your GDT
    descriptor->attributes     = flags;
    descriptor->isr_high       = (uint32_t)isr >> 16;
    descriptor->reserved       = 0;
}
char *idt_descriptions[32] = {
    "Dividing error",
    "Debug",
    "Nonmaskable External Interrupt",
    "Breakpoint (int3)",
    "Overflow (into)",
    "BOUND Range Exceeded",
    "Invalid (Undefined) opcode",
    "Device Not Available (WAIT/FWAIT)",
    "Double Fault",
    "Coprocessor Segment Overrun",
    "Invalid TSS Segment",
    "Segment Not Present",
    "Stack Segment Fault",
    "General Protection Fault",
    "Page Fault",
    "Reserved (Inexplicable)",
    "x87 Floating Point Error",
    "Alignment Check",
    "Machine Check",
    "SIMD Floating Point Exception",
    "Virtualization Exception",
    "Control Protection Execution",
    "Reserved (Inexplicable)",
    "Reserved (Inexplicable)",
    "Reserved (Inexplicable)",
    "Reserved (Inexplicable)",
    "Reserved (Inexplicable)",
    "Reserved (Inexplicable)",
    "Reserved (Inexplicable)",
    "Reserved (Inexplicable)",
    "Reserved (Inexplicable)",
    "Reserved (Inexplicable)"
};
void idt_init() {
    idtr.base = (uintptr_t) &idt[0];
    idtr.limit = (uint16_t) sizeof(idt_entry_t) * IDT_MAX_DESCRIPTORS - 1;

    for (uint8_t vector = 0; vector < 32; vector++) {
        idt_set_descriptor(vector, ISR_TABLE[vector], 0x8E);
        vectors[vector] = true;
    }

    __asm__ volatile ("lidt %0" : : "m"(idtr)); // load the new IDT
    __asm__ volatile ("sti"); // set the interrupt flag
}
void ERR_IDT_HANDLER(idt_pushed_regs_t regs) {
    klog("INT %d: %s with code %d",regs.interrupt_code,idt_descriptions[regs.interrupt_code],regs.error_code);
    klog("Begin pushed reg dump.\nds=%x\nedi=%x\nesi=%x\nebp=%x\nesp=%x\nebx=%x\nedx=%x\necx=%x\neax=%x\ni_code=%x\ne_code=%x\neip=%x\ncs=%x\neflags=%x\nuesp=%x\nss=%x\n",
    regs.dataseg,regs.edi,regs.esi,regs.esp,regs.ebx,regs.edx,regs.ecx,regs.eax,regs.interrupt_code,regs.error_code,regs.eip,regs.codeseg,regs.eflags,regs.useresp,regs.ss);
    __asm__ volatile ("cli; hlt"); // Completely hangs the computer
}
void NOERR_IDT_HANDLER(idt_pushed_regs_t regs) {
    klog("INT %d: %s",regs.interrupt_code, idt_descriptions[regs.interrupt_code]);
    return;
}