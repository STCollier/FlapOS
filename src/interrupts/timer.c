#include "timer.h"
#include "../util.h"
#include "isr.h"
#include "../bananas/vga.h"

static uint64_t _tick = 0;

static void timer_callback([[maybe_unused]] registers_t regs) {
    _tick++;
}

void timer_init(uint64_t hz) {
    register_interrupt_handler(IRQ0, timer_callback);

    /* Get the PIT value: hardware clock at 1193180 Hz */
    uint64_t divisor = 1193180 / hz;
    uint8_t low  = (uint8_t)(divisor & 0xFF);
    uint8_t high = (uint8_t)((divisor >> 8) & 0xFF);

    outportb(PIT_CMD, 0x36);
    outportb(PIT0, low);
    outportb(PIT0, high);
}

uint64_t tick() {
    return _tick;
}
