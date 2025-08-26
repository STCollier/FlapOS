#include "util.h"
#include "idt.h"
#include "vga.h"
//__attribute__((noreturn))
void ERR_IDT_HANDLER(uint32_t code) {
    klog("Panic: IDT Error code %d: %s",code,idt_descriptions[code]);
    __asm__ volatile ("cli; hlt"); // Completely hangs the computer
    return;
}
void NOERR_IDT_HANDLER(uint32_t code) {
    klog("NOERR INT %d: %s",code, idt_descriptions[code]);
    return;
}

void kmain(void) {
    
    memset(VGA, 0x1, 320 * 200); // clear vga mem
    klog("Kernel loaded");
    klog("Load IDT");
    idt_init();
    
    klog("IDT loaded");
    __asm__ volatile ("int $1");
    klog("Interrupt 15 test finished");
    
    while(1){__asm__ volatile ("hlt");};

}
