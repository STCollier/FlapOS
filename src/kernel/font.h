#ifndef FONT_H
#define FONT_H

#include "util.h"

extern uint8_t* VGA;

void printc(char c, size_t x, size_t y);
void prints(char* str, size_t x, size_t y);

#endif