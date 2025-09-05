#include "../drivers/vga.h"
#include "../util.h"
#include "bird.h"

static const uint8_t downflap[12][17] = {
    { 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0 },
    { 0, 0, 0, 0, 1, 1, 2, 2, 2, 1, 3, 3, 1, 0, 0, 0, 0 },
    { 0, 0, 0, 1, 2, 2, 4, 4, 1, 3, 3, 3, 3, 1, 0, 0, 0 },
    { 0, 0, 1, 2, 4, 4, 4, 4, 1, 5, 3, 3, 1, 3, 1, 0, 0 },
    { 0, 1, 4, 4, 4, 4, 4, 4, 1, 5, 3, 3, 1, 3, 1, 0, 0 },
    { 0, 1, 4, 4, 4, 4, 4, 4, 4, 1, 5, 3, 3, 3, 1, 0, 0 },
    { 0, 1, 1, 1, 1, 1, 4, 4, 4, 4, 1, 1, 1, 1, 1, 1, 0 },
    { 1, 2, 3, 3, 3, 2, 1, 4, 4, 1, 6, 6, 6, 6, 6, 6, 1 },
    { 1, 3, 3, 3, 3, 1, 7, 7, 1, 6, 1, 1, 1, 1, 1, 1, 0 },
    { 1, 3, 3, 2, 1, 7, 7, 7, 7, 1, 6, 6, 6, 6, 6, 1, 0 },
    { 0, 1, 1, 1, 1, 7, 7, 7, 7, 7, 1, 1, 1, 1, 1, 0, 0 },
    { 0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0 }
};

static const uint8_t midflap[12][17] = {
    { 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0 },
    { 0, 0, 0, 0, 1, 1, 2, 2, 2, 1, 3, 3, 1, 0, 0, 0, 0 },
    { 0, 0, 0, 1, 2, 2, 4, 4, 1, 3, 3, 3, 3, 1, 0, 0, 0 },
    { 0, 0, 1, 2, 4, 4, 4, 4, 1, 5, 3, 3, 1, 3, 1, 0, 0 },
    { 0, 1, 4, 4, 4, 4, 4, 4, 1, 5, 3, 3, 1, 3, 1, 0, 0 },
    { 0, 1, 1, 1, 1, 1, 4, 4, 4, 1, 5, 3, 3, 3, 1, 0, 0 },
    { 1, 3, 3, 3, 3, 3, 1, 4, 4, 4, 1, 1, 1, 1, 1, 1, 0 },
    { 1, 2, 3, 3, 3, 2, 1, 4, 4, 1, 6, 6, 6, 6, 6, 6, 1 },
    { 0, 1, 1, 1, 1, 1, 7, 7, 1, 6, 1, 1, 1, 1, 1, 1, 0 },
    { 0, 0, 1, 7, 7, 7, 7, 7, 7, 1, 6, 6, 6, 6, 6, 1, 0 },
    { 0, 0, 0, 1, 1, 7, 7, 7, 7, 7, 1, 1, 1, 1, 1, 0, 0 },
    { 0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0 }
}; 

static const uint8_t upflap[12][17] = {
    { 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0 },
    { 0, 0, 0, 0, 1, 1, 2, 2, 2, 1, 3, 3, 1, 0, 0, 0, 0 },
    { 0, 0, 0, 1, 2, 2, 4, 4, 1, 3, 3, 3, 3, 1, 0, 0, 0 },
    { 0, 1, 1, 1, 1, 4, 4, 4, 1, 5, 3, 3, 1, 3, 1, 0, 0 },
    { 1, 3, 3, 3, 3, 1, 4, 4, 1, 5, 3, 3, 1, 3, 1, 0, 0 },
    { 1, 3, 3, 3, 3, 3, 1, 4, 4, 1, 5, 3, 3, 3, 1, 0, 0 },
    { 1, 2, 3, 3, 3, 2, 1, 4, 4, 4, 1, 1, 1, 1, 1, 1, 0 },
    { 0, 1, 2, 2, 2, 1, 4, 4, 4, 1, 6, 6, 6, 6, 6, 6, 1 },
    { 0, 0, 1, 1, 1, 7, 7, 7, 1, 6, 1, 1, 1, 1, 1, 1, 0 },
    { 0, 0, 1, 7, 7, 7, 7, 7, 7, 1, 6, 6, 6, 6, 6, 1, 0 },
    { 0, 0, 0, 1, 1, 7, 7, 7, 7, 7, 1, 1, 1, 1, 1, 0, 0 },
    { 0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0 }
}; 

struct Bird bird_init() {
    struct Bird bird;

    bird.size =  (vec2_t) {sizeof(midflap[0]) / sizeof(midflap[0][0]), sizeof(midflap) / sizeof(midflap[0])};
    bird.pos =   (vec2_t) {VGA_WIDTH / 2, VGA_HEIGHT / 2 - 50};
    bird.vel =   FVEC2_ZERO;
    bird.acc =   FVEC2_ZERO;
    bird.frame = FLAP_MIDDLE;
    bird.dead = false;
    bird.score = 0;

    return bird;
}

static size_t counter = 0;

void bird_draw(struct Bird* bird, uint64_t tick) {
    if (!(tick % 3)) {
        bird->frame = (int[]){0, 1, 2, 1}[counter++ % 4];
    }
    putpixelmatrix(
        bird->pos, bird->size, 0,
        bird->frame == FLAP_DOWN ? (uint8_t*) downflap :
        bird->frame == FLAP_MIDDLE?(uint8_t*) midflap :
        (uint8_t*) upflap 
    );

    bird->acc.y += .3f;
    bird->vel.y += bird->acc.y;
    bird->pos.y += roundToInt(bird->vel.y);
    if (bird->pos.y < 0) {
        bird->pos.y = 0;
        bird->vel.y = 0;
    }
    bird->acc.y = 0;

}

void bird_flap(struct Bird* bird) {
    if (!bird->dead) {
        bird->acc.y = 0;
        bird->vel.y = -5;
    }
}

static char score[9];

void bird_drawScore(struct Bird* bird) {
    kprints("Score: ", 6, 6, 0xFE);
    kprints("Score: ", 5, 5, 0xFF);

    kprints(itoa(bird->score, score, 10), 56, 6, 0xFE);
    kprints(itoa(bird->score, score, 10), 55, 5, 0xFF);
}

void bird_checkCollision(struct Bird* bird, vec2_t ppos, vec2_t pdim) {
    bool hit = rrcollide(bird->pos, bird->size, ppos, pdim);
    if (hit || bird->pos.y >= VGA_HEIGHT) bird->dead = true;
}