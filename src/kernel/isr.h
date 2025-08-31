#ifndef ISR_H
#define ISR_H

#include "util.h"

#define PIC1 0x20 // IO master
#define PIC1_OFFSET 0x20
#define PIC1_DATA (PIC1 + 1)

#define PIC2 0xA0 // IO slave
#define PIC2_OFFSET 0x28
#define PIC2_DATA (PIC2 + 1)

#define PIC_EOI 0x20
#define PIC_MODE_8086 0x01
#define ICW1_ICW4 0x01
#define ICW1_INIT 0x10

#define IRQ0  32
#define IRQ1  33
#define IRQ2  34
#define IRQ3  35
#define IRQ4  36
#define IRQ5  37
#define IRQ6  38
#define IRQ7  39
#define IRQ8  40
#define IRQ9  41
#define IRQ10 42
#define IRQ11 43
#define IRQ12 44
#define IRQ13 45
#define IRQ14 46
#define IRQ15 47

extern void ISR0();
extern void ISR1();
extern void ISR2();
extern void ISR3();
extern void ISR4();
extern void ISR5();
extern void ISR6();
extern void ISR7();
extern void ISR8();
extern void ISR9();
extern void ISR10();
extern void ISR11();
extern void ISR12();
extern void ISR13();
extern void ISR14();
extern void ISR15();
extern void ISR16();
extern void ISR17();
extern void ISR18();
extern void ISR19();
extern void ISR20();
extern void ISR21();
extern void ISR22();
extern void ISR23();
extern void ISR24();
extern void ISR25();
extern void ISR26();
extern void ISR27();
extern void ISR28();
extern void ISR29();
extern void ISR30();
extern void ISR31();

extern void _IRQ0();
extern void _IRQ1();
extern void _IRQ2();
extern void _IRQ3();
extern void _IRQ4();
extern void _IRQ5();
extern void _IRQ6();
extern void _IRQ7();
extern void _IRQ8();
extern void _IRQ9();
extern void _IRQ10();
extern void _IRQ11();
extern void _IRQ12();
extern void _IRQ13();
extern void _IRQ14();
extern void _IRQ15();

static const char* EXCEPTIONS[32] = {
   "Division By Zero",
   "Debug",
   "Non Maskable Interrupt",
   "Breakpoint",
   "Into Detected Overflow",
   "Out of Bounds",
   "Invalid Opcode",
   "No Coprocessor",

   "Double Fault",
   "Coprocessor Segment Overrun",
   "Bad TSS",
   "Segment Not Present",
   "Stack Fault",
   "General Protection Fault",
   "Page Fault",
   "Unknown Interrupt",

   "Coprocessor Fault",
   "Alignment Check",
   "Machine Check",
   "Reserved",
   "Reserved",
   "Reserved",
   "Reserved",
   "Reserved",

   "Reserved",
   "Reserved",
   "Reserved",
   "Reserved",
   "Reserved",
   "Reserved",
   "Reserved",
   "Reserved"
};

static const char* IRQ_DESC[16] = {
   "PIT",
   "Keyboard",
   "Cascade",
   "COM2",
   "COM1",
   "LPT2",
   "Floppy Disk",
   "LPT1 / Unreliable",
   "CMOS real-time clock"
   "Free",
   "Free",
   "Free",
   "PS2 Mouse",
   "FPU / Coprocessor/ Inter-processor",
   "Primary ATA Hard Disk",
   "Secondary ATA Hard Disk"
};

typedef struct {
   uint32_t ds; /* Data segment selector */
   uint32_t edi, esi, ebp, esp, ebx, edx, ecx, eax; /* Pushed by pusha. */
   uint32_t int_no, err_code; /* Interrupt number and error code (if applicable) */
   uint32_t eip, cs, eflags, useresp, ss; /* Pushed by the processor automatically */
} registers_t;

void isr_init();
void isr_handler(registers_t r);
void irq_init();
//void irq_handler(registers_t r);

typedef void (*isr_t)(registers_t);
void register_interrupt_handler(uint8_t n, isr_t handler);


#endif
