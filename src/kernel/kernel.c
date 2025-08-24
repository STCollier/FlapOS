#include "util.h"
#include "font.h"
#include "graphics.h"
#include "idt.h"

__attribute__((noreturn))
void exception_handler() {
    VGA = (uint8_t*) 0xA0000;
    memset(VGA, 0x4, 320 * 200);
    kprints_nowrap("Exception triggered!", 8, 8);
    __asm__ volatile ("cli; hlt"); // Completely hangs the computer
}

void kmain(void) {
    //idt_init();

    VGA = (uint8_t*) 0xA0000;
    //VGA_setPalette(); // test set palette index 0x1 as red instead of default BIOS blue

    memset(VGA, 0x1, 320 * 200);
    
    kprintf("Hello Test\n");
    kprintf("%s","linewraplinewraplinewraplinewraplinewraplinewraplinewraplinewraplinewraplinewraplinewrap");
    kprintf("0x50+0x90=%x\n",0x50+0x90);
    kprintf("String test: %s\n","passed\n");
    kprintf("%cello world%s",'H',"\n");
    kprintf("pancake is the best amirite\n");
    kprintf("Regular number test: %d %d %d %d %d %d %d %d %d %d %d\n",1,12,123,1234,12345,123456,1234567,12345678,123456789,1234567890, -69420);
    // issue: we should output to a serial port and capture the output in qemu so we dont lose data when the screen is filled.
    while(1);

}
