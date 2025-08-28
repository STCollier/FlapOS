#ifndef IDT_H
#define IDT_H

#include "util.h"

#define IDT_MAX_DESCRIPTORS 256

typedef struct {
	uint16_t    isr_low;      // The lower 16 bits of the ISR's address
	uint16_t    kernel_cs;    // The GDT segment selector that the CPU will load into CS before calling the ISR
	uint8_t     reserved;     // Set to zero
	uint8_t     attributes;   // Type and attributes; see the IDT page
	uint16_t    isr_high;     // The higher 16 bits of the ISR's address
} __attribute__((packed)) idt_entry_t;

__attribute__((aligned(0x10))) 
static idt_entry_t idt[256]; // Create an array of IDT entries; aligned for performance

typedef struct {
	uint16_t	limit;
	uint32_t	base;
} __attribute__((packed)) idtr_t;


extern uint32_t* ISR_TABLE[];
extern char* idt_descriptions[];

static bool vectors[IDT_MAX_DESCRIPTORS];
static idtr_t idtr;

typedef struct {
	uint32_t dataseg;
	uint32_t edi,esi,ebp,esp,ebx,edx,ecx,eax;
	uint32_t interrupt_code;
	uint32_t error_code;
	uint32_t eip,codeseg,eflags,useresp,ss;
} __attribute__((packed)) idt_pushed_regs_t; // data structure for raw pushed data once called by idt

//void ERR_IDT_HANDLER(idt_pushed_regs_t regs);
//void NOERR_IDT_HANDLER(idt_pushed_regs_t regs);
void isr_handle(idt_pushed_regs_t regs);
void idt_set_descriptor(uint8_t vector, void* isr, uint8_t flags);
void idt_init();

#endif