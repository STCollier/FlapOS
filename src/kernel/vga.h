#ifndef VGA_H
#define VGA_H
extern size_t KPRINTF_CURRENT_X;
extern size_t KPRINTF_CURRENT_Y;
extern uint8_t*VGA;
void VGA_setPalette();
void VGA_setColor(uint8_t idx, uint8_t r, uint8_t g, uint8_t b);
void kprintc(char c, size_t x, size_t y);
void kprints(char* str, size_t x, size_t y);
bool kprintf(char *format, ...);
bool klog(char *format, ...);
#endif