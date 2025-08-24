#include "util.h"
#include "idt.h"
#include "vga.h"
__attribute__((noreturn))
void exception_handler() {
    VGA = (uint8_t*) 0xA0000;
    memset(VGA, 0x4, 320 * 200);
    kprints("Exception triggered!", 8, 8);
    __asm__ volatile ("cli; hlt"); // Completely hangs the computer
}

void kmain(void) {
    //idt_init();

    VGA = (uint8_t*) 0xA0000;
    //VGA_setPalette(); // test set palette index 0x1 as red instead of default BIOS blue

    memset(VGA, 0x1, 320 * 200);
    
    klog("Kernel loaded%c",'.');
    
    // issue: we should output to a serial port and capture the output in qemu so we dont lose data when the screen is filled.
    while(1);

}
