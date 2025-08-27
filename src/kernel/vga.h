#ifndef VGA_H
#define VGA_H
#include "util.h"
extern uint8_t* VGA;
static const bool ENABLE_SCROLLING = true;
void VGA_setPalette();
void VGA_setColor(uint8_t idx, uint8_t r, uint8_t g, uint8_t b);
void kprintc(char c, size_t x, size_t y);
void kprints(char* str, size_t x, size_t y);
bool kprintf(char *format, ...);
bool klog(char *format, ...);
#endif