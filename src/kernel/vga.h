#ifndef VGA_H
#define VGA_H

#include "util.h"

extern uint8_t* VGA;

void VGA_setPalette();
void VGA_setColor(uint8_t idx, uint8_t r, uint8_t g, uint8_t b);
void kprintc(char c, size_t x, size_t y);
void kprints(const char* str, size_t x, size_t y);
bool kprintf(const char *format, ...);
bool klog(const char *format, ...);

void putpixel(uint8_t color, size_t x, size_t y);
void putpixelmatrix(uint16_t begin_x, uint16_t begin_y, uint16_t size_x, uint16_t size_y, uint8_t nowrite_byte, uint8_t *matrix);
#endif
