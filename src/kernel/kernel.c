#include "util.h"
#include "font.h"
#include "graphics.h"

void kmain(void) {
    VGA = (uint8_t*) 0xA0000;

    VGA_setPalette(); // test set palette index 0x1 as red instead of default BIOS blue

    memset(VGA, 0x1, 320 * 200);

    prints("Hello World!", 8, 8);
}