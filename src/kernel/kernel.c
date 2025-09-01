#include "util.h"
#include "vga.h"
#include "isr.h"
#include "timer.h"
#include "bird.h"


void kmain(void) {
    VGA_setPalette();
    memset(VGA, 0, WIDTH * HEIGHT);
    
    klog("Kernel loaded.");
    
    klog("Load ISR.");
    isr_init();

    klog("Load IRQ.");
    irq_init();

    TEST_INTERRUPT(1)

    int x = 0;
    uint64_t t = 0;
    while (true) {
        if (tick() != t) {
            t = tick();

            if (t % 5 == 0) {
                bird_draw(x++ % 3);
            }

            //klog("Tick: %d", t);
        }
    }
}
