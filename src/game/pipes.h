#ifndef PIPE_H
#define PIPE_H

#include "../util.h"
#include "bird.h"

struct Pipes {
    int x;
    int offset;
    bool passed;
};

void pipes_init();
void pipes_draw();
void pipes_update(struct Bird* bird, uint64_t tick);

#endif