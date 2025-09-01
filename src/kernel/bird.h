#ifndef BIRD_H
#define BIRD_H

enum FlapState {
    FLAP_DOWN,
    FLAP_MIDDLE,
    FLAP_UP
};

void bird_draw(enum FlapState frame);

#endif