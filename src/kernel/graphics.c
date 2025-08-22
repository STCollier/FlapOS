#include "graphics.h"

/*
See: https://wiki.osdev.org/VGA_Hardware#Port_0x3C8

To write a color, write the color index to port 0x3C8, then write 3 bytes to 0x3C9 in the order red, green, blue. 
If you want to write multiple consecutive DAC entries, you only need to write the first entry's index to 0x3C8 then write 
all values to 0x3C9 in the order red, green, blue, red, green, blue, and so on. 
The accessed DAC entry will automatically increment after every three bytes written

NOTE: colors are in the range [0, 63]
*/

uint8_t* VGA = 0;

void VGA_setColor(uint8_t idx, uint8_t r, uint8_t g, uint8_t b) {
    // TODO: assert impl
    // assert(idx < 256 && r < 64 && g < 64 && b < 64);

    outportb(0x3C8, idx);
    outportb(0x3C9, r);
    outportb(0x3C9, g);
    outportb(0x3C9, b);
}

void VGA_setPalette() {
    // We can either generate a palette with a loop, or select certain colors that pertain to the art we want

    VGA_setColor(0x1, 63, 0, 0);
}