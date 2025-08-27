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

#endif