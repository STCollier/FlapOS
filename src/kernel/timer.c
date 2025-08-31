#include "timer.h"
#include "util.h"
#include "isr.h"
#include "vga.h"

uint32_t tick = 0;

static void timer_callback(registers_t regs) {
    tick++;

    klog("Tick: %d", tick);
}

// timer impl stolen from cfenollosa for testing; will redo later 

void timer_init(uint32_t hz) {
    /* Install the function we just wrote */
    register_interrupt_handler(IRQ0, timer_callback);

    /* Get the PIT value: hardware clock at 1193180 Hz */
    uint32_t divisor = 1193180 / hz;
    uint8_t low  = (uint8_t)(divisor & 0xFF);
    uint8_t high = (uint8_t)((divisor >> 8) & 0xFF);
    /* Send the command */
    outportb(0x43, 0x36); /* Command port */
    outportb(0x40, low);
    outportb(0x40, high);
}
