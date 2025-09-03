#include "util.h"
#include "vga.h"
#include "isr.h"
#include "timer.h"
#include "keyboard.h"
#include "bird.h"
#include "pipes.h"

enum Scene {
    SCENE_LOAD,
    SCENE_MENU,
    SCENE_GAME
};

void load() {

}

void kmain(void) {
    enum Scene scene = SCENE_LOAD;

    VGA_setPalette();
    
    klog("Load ISR.");
    isr_init();

    klog("Load IRQ.");
    irq_init();

    TEST_INTERRUPT(1)

    struct Bird bird = bird_init();

    pipes_init();
    klog("Ready. Press [S] to run.");
    VGA_swap();

    bool pressed = false;
    uint64_t t = 0;

    while (true) {

        if (tick() != t) {
            t = tick();
            
            if (scene == SCENE_LOAD) {
                if (key_pressed(KEY_S)) scene = SCENE_MENU;
            }

            if (scene == SCENE_MENU) {
                kprints("FLAPPY BIRD", 160-88, 16, 255);
                //kprints("FLAPPY BIRD", 160-88, 16, 255); // try uncommenting
            }

            if (scene == SCENE_GAME) {
                if (key_pressed(KEY_SPACE)) {
                    if (!pressed) {
                        bird_flap(&bird);
                        pressed = true;
                    }
                } else pressed = false;

                bird_draw(&bird, t);
                pipes_draw(t);
                bird_drawScore(t);
            }

            VGA_swap();
            if (scene != SCENE_LOAD) VGA_clear();
        }
    }
}
