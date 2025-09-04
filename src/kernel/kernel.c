#include "../util.h"
#include "../drivers/vga.h"
#include "../interrupts/isr.h"
#include "../interrupts/timer.h"
#include "../drivers/keyboard.h"
#include "../game/bird.h"
#include "../game/pipes.h"

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
    bool mflip = false;

    while (true) {

        if (tick() != t) {
            t = tick();
            
            if (scene == SCENE_LOAD) {
                if (key_pressed(KEY_S)) {scene = SCENE_MENU; continue;}
            }

            if (scene == SCENE_MENU) {
                Lkprints("FLAPPY BIRD", 160-90, 18, 253);
                Lkprints("FLAPPY BIRD", 160-88, 16, 255);

                kprints("Press [ENTER] to Play", 160-89, 151, mflip ? 252 : 251);
                kprints("Press [ENTER] to Play", 160-88, 150, mflip ? 251 : 252);

                if (!(t % 15)) mflip = !mflip;

                if (key_pressed(KEY_ENTER)) scene = SCENE_GAME;
            }

            if (scene == SCENE_GAME) {
                if (key_pressed(KEY_SPACE)) {
                    if (!pressed) {
                        bird_flap(&bird);
                        pressed = true;
                    }
                } else pressed = false;

                bird_draw(&bird, t);
                pipes_update(&bird, t);
                pipes_draw();
                
                bird_drawScore(t);
            }

            VGA_swap();

            if (scene != SCENE_LOAD) VGA_clear(); 
          }
    }
}
