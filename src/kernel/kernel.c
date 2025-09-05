#include "../util.h"
#include "../drivers/vga.h"
#include "../interrupts/isr.h"
#include "../drivers/timer.h"
#include "../drivers/keyboard.h"
#include "../game/bird.h"
#include "../game/pipes.h"

enum Scene {
    SCENE_LOAD,
    SCENE_MENU,
    SCENE_GAME
};

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
                struct Bird dummy = { .pos = { -999, -999 }, .dead = false };

                pipes_update(&dummy, t);
                pipes_draw();

                Lkprints("FLAPPY BIRD", 160-90, 18, 253);
                Lkprints("FLAPPY BIRD", 160-88, 16, 255);

                kprints("Press [ENTER] to Play", 160-89, 151, mflip ? 252 : 251);
                kprints("Press [ENTER] to Play", 160-88, 150, mflip ? 251 : 252);

                if (key_pressed(KEY_ENTER)) {
                    pipes_init();
                    bird = bird_init();

                    scene = SCENE_GAME;
                }
            }

            if (!(t % 15)) mflip = !mflip;

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
                
                bird_drawScore(&bird);

                if (bird.dead) {
                    VGA_rect((vec2_t) {VGA_WIDTH / 2 - 47, 33}, (vec2_t) {100, 30}, 249);
                    VGA_rect((vec2_t) {VGA_WIDTH / 2 - 50, 30}, (vec2_t) {100, 30}, 250);

                    kprints("You Died!", VGA_WIDTH / 2 - 35, 42, 254);
                    kprints("You Died!", VGA_WIDTH / 2 - 36, 41, 255);

                    kprints("[X] to return to menu", VGA_WIDTH / 2 - 87, 71, mflip ? 248 : 247);
                    kprints("[X] to return to menu", VGA_WIDTH / 2 - 88, 70, mflip ? 247 : 248);

                    kprints("[ENTER] to restart", VGA_WIDTH / 2 - 75, 81, mflip ? 248 : 247);
                    kprints("[ENTER] to restart", VGA_WIDTH / 2 - 76, 80, mflip ? 247 : 248);

                    if (key_pressed(KEY_X)) {
                        scene = SCENE_MENU;
                    }

                    if (key_pressed(KEY_ENTER)) {
                        pipes_init();
                        bird = bird_init();
                    }
                }
            }

            VGA_swap();

            if (scene != SCENE_LOAD) VGA_clear(); 
        }
    }
}
