#include "util.h"
#include "idt.h"
#include "vga.h"
//__attribute__((noreturn))
void ERR_IDT_HANDLER(uint32_t code) {
    VGA = (uint8_t*) 0xA0000;
    klog("Panic: IDT Error code %d",code);
    __asm__ volatile ("cli; hlt"); // Completely hangs the computer
}
void NOERR_IDT_HANDLER(uint32_t code) {
    __asm__ volatile ("cli");
    return;
}

void kmain(void) {
    idt_init();

    VGA = (uint8_t*) 0xA0000;
    //VGA_setPalette(); // test set palette index 0x1 as red instead of default BIOS blue

    memset(VGA, 0x1, 320 * 200);
    
    klog("Kernel loaded%c",'.');
    // automatically goes to double fault. this is when a regular exception handler doesnt properly handle an exception and thus makes its own exception. 
    //idk how to un interrupt in the no-error idt handler so thats probably the issue.
    //klog("Test IDT div 0 error");
    //klog("%d\n",10/0);
    // issue: we should output to a serial port and capture the output in qemu so we dont lose data when the screen is filled.
    while(1);

}
