#include "util.h"
#include "vga.h"
#include "isr.h"
#include "timer.h"
#include "keyboard.h"
#include "bird.h"
#include "pipes.h"


void kmain(void) {
    VGA_setPalette();
    
    klog("Kernel loaded.");
    
    klog("Load ISR.");
    isr_init();

    klog("Load IRQ.");
    irq_init();

    TEST_INTERRUPT(1)

    struct Bird bird = bird_init();
    
    pipes_init();
    klog("Ready. Press [S] to run.");

        VGA_swap();
    bool started = false;
    bool pressed = false;
    uint64_t t = 0;
    while (true) {

        if (tick() != t) {
            t = tick();
            if (!started && key_pressed(KEY_S)) {
                started = true;
            }
            if (!started) continue;
            if (key_pressed(KEY_SPACE)) {
                if (!pressed) {
                    bird_flap(&bird);
                    pressed = true;
                }
            } else {
                pressed = false;
            }

            VGA_clear();

            bird_draw(&bird, t);
            pipes_draw(t);

            VGA_swap();
        }
    }
}
