#include "util.h"
#include "font.h"

uint8_t* VGA;

void kmain(void) {
    VGA = (uint8_t*) 0xA0000;

    memset(VGA, 0x1, 320 * 200);

    prints("Hello World!", 8, 8);
}