#include "util.h"
#include "vga.h"
#include "isr.h"
#include "timer.h"
#include "keyboard.h"
#include "bird.h"
#include "pipes.h"


void kmain(void) {
    VGA_setPalette();
    memset(VGA, 0, WIDTH * HEIGHT);
    
    klog("Kernel loaded.");
    
    //klog("Load ISR.");
    isr_init();

    //klog("Load IRQ.");
    irq_init();

    //TEST_INTERRUPT(1)

    struct Bird bird = bird_init();
    pipes_init();

    bool pressed = false;
    uint64_t t = 0;
    while (true) {
        if (tick() != t) {
            t = tick();

            if (key_pressed(KEY_SPACE)) {
                if (!pressed) {
                    bird_flap(&bird);
                    pressed = true;
                }
            } else {
                pressed = false;
            }

            memset(VGA, 0, WIDTH * HEIGHT);

            bird_draw(&bird, t);
            pipes_draw(t);

            //klog("Tick: %d", t);
        }
    }
}
