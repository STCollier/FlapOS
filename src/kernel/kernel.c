void kmain(void) {
    unsigned char *vga = (unsigned char*) 0xA0000;

    unsigned int rainbow[6] = {4,6,14,2,9,5};
    int i = 0;
    for (int x = 0; x < 320; x++) {
        for (int y = 0; y < 200; y++) {
            unsigned int color = rainbow[i++ % (sizeof(rainbow) / 4)];
            vga[y * 320 + x] = color;
        }
    }

    while (1);
}