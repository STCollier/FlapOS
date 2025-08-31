#include "util.h"
#include "isr.h"
#include "vga.h"
#include "keyboard.h"

static void key_callback(registers_t regs) {
    enum Scancode key = inb(0x60);

    if (key == SPACE) {
        klog("Pressed space!");
    }
}

void keyboard_init() {
    register_interrupt_handler(IRQ1, key_callback);
}

