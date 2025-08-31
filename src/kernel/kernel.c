#include "util.h"
#include "vga.h"
#include "isr.h"
#include "timer.h"

#define TI(x)                                  \
    klog("Call interrupt %d.", x);             \
    __asm__ volatile("int %0" :: "i"(x));


void kmain(void) {
    memset(VGA, 0x1, WIDTH * HEIGHT);
    
    klog("Kernel loaded.");
    
    klog("Load ISR.");
    isr_init();

    klog("Load IRQ.");
    irq_init();

    klog("Test interrupt 0x1");
    TI(1)

    klog("Try pressing space");

    uint64_t t = 0;
    while (true) {
        if (tick() != t) {
            t = tick();

            //klog("Tick: %d", t);
        }
    }
}
