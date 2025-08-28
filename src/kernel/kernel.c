#include "util.h"
#include "idt.h"
#include "vga.h"


void kmain(void) {
    memset(VGA, 0x1, 320 * 200); // clear vga mem
    
    klog("Kernel loaded.");
    klog("Load IDT.");
    idt_init();
    
    klog("IDT loaded.");
    __asm__ volatile ("int $1");
    klog("Interrupt 1 test finished.");
    
    /*
    
    https://wiki.osdev.org/Interrupt_Vector_Table#Default_Hardware_Interrupt_Layout
    https://stackoverflow.com/questions/79230135/kernel-double-fault-when-loading-idt-os-dev
    https://wiki.osdev.org/8259_PIC
    */ 
    // halting is outside the function
    
}
