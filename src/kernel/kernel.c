#include "util.h"
#include "font.h"
#include "graphics.h"
#include "idt.h"

__attribute__((noreturn))
void exception_handler() {
    __asm__ volatile ("cli; hlt"); // Completely hangs the computer
}

void kmain(void) {
    idt_init();

    VGA = (uint8_t*) 0xA0000;

    //VGA_setPalette(); // test set palette index 0x1 as red instead of default BIOS blue

    memset(VGA, 0x1, 320 * 200);

    prints("Hello World!", 8, 8);
}