#ifndef GRAPHICS_H
#define GRAPHICS_H

#include "util.h"

extern uint8_t* VGA;

void VGA_setColor(uint8_t idx, uint8_t r, uint8_t g, uint8_t b);
void VGA_setPalette();

#endif