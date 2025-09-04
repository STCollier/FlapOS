#include "../util.h"
#include "../interrupts/isr.h"
#include "vga.h"
#include "keyboard.h"

struct Keyboard keyboard;

static void key_callback([[maybe_unused]] registers_t regs) {
    uint8_t scancode = inb(0x60);

    if (scancode & 0x80) {
        // Release
        scancode &= 0x7F;
        keyboard.keys[scancode] = false;
    } else {
        // Press
        keyboard.keys[scancode] = true;
    }
}

void keyboard_init() {
    register_interrupt_handler(IRQ1, key_callback);
    memset(keyboard.keys, false, 128);
}

bool key_pressed(enum Scancode key) {
    return keyboard.keys[key];
}
