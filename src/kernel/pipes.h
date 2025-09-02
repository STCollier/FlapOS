#ifndef PIPE_H
#define PIPE_H

#include "util.h"

struct Pipes {
    int x;
    int offset;
};

void pipes_init();
void pipes_draw(uint64_t tick);

#endif