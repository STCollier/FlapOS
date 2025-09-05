#ifndef BIRD_H
#define BIRD_H

enum FlapState {
    FLAP_DOWN,
    FLAP_MIDDLE,
    FLAP_UP
};

struct Bird {
    vec2_t pos, size;
    fvec2_t vel, acc;
    enum FlapState frame;
    bool dead;
    size_t score;
};

struct Bird bird_init();
void bird_draw(struct Bird* bird, uint64_t tick);
void bird_flap(struct Bird* bird);
void bird_drawScore(struct Bird* bird);
void bird_checkCollision(struct Bird* bird, vec2_t ppos, vec2_t pdim);

#endif