#include "util.h"
#include "vga.h"
#include "isr.h"
#include "idt.h"

#define TI(x)                      \
    klog("Call interrupt %d.", x); \
    __asm__ volatile("int $" #x);  \


void kmain(void) {
    memset(VGA, 0x1, 320 * 200);
    
    klog("Kernel loaded.");
    
    isr_init();
    irq_init();
    
    TI(0)
    TI(1)
    TI(2)
}
