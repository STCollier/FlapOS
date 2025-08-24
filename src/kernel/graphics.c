#include "graphics.h"
#include "font.h"
#include <stdarg.h> // yeah i dont know either
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
size_t KPRINTF_CURRENT_X = 0;
size_t KPRINTF_CURRENT_Y = 0;
bool   FILLED_SCREEN = false;
static void checkwrapping() {
    if (KPRINTF_CURRENT_X == 40) {
        KPRINTF_CURRENT_X=0;
        KPRINTF_CURRENT_Y++;
    }
    if (KPRINTF_CURRENT_Y==20){
        FILLED_SCREEN=true;
        KPRINTF_CURRENT_Y=0;
    }
}
char* kitoa(int num, char* str, int base)
{
    int i = 0;
    bool neg = false;
    if (num == 0) {
        str[i++] = '0';
        str[i] = '\0';
        return str;
    }
    if (num < 0 && base == 10) {
        neg = true;
        num = -num;
    }

    while (num != 0) {
        int rem = num % base;
        str[i++] = (rem > 9) ? (rem - 10) + 'a' : rem + '0';
        num = num / base;
    }
    if (neg)
        str[i++] = '-';
    str[i] = '\0';
    int start = 0;
    int end = i - 1;
    while (start < end) {
        char temp = str[start];
        str[start] = str[end];
        str[end] = temp;
        end--;
        start++;
    }
    return str;
}
bool kprintf(char *format, ...) {
    va_list args;
    va_start(args, format);
    
    bool in_specifier = false;
    for (int i = 0; *(format+i); i++) {
        char current = *(format+i);
        if (in_specifier) {
            switch(current) {
                case 'c':
                   uint32_t let = va_arg(args, uint32_t); // idk why but it gets really angry if its not a u32 passed in.
                   // id have to guess its some weirdness with the fact that im passing in stdarg.h from my linux system into a baremetal os but what do i know
                   kprintc((uint8_t)let, KPRINTF_CURRENT_X*8, KPRINTF_CURRENT_Y*10);
                   KPRINTF_CURRENT_X++;
                   checkwrapping();
                   break;
                case 's':
                    char *ptr = va_arg(args,char*);
                    for(int j =  0; *(ptr+j);j++) {
                        if (*(ptr+j)=='\n'){
                            KPRINTF_CURRENT_Y++;
                            KPRINTF_CURRENT_X=0;
                            checkwrapping(); // filled screen check
                            continue;
                        }
                        kprintc(*(ptr+j), KPRINTF_CURRENT_X*8, KPRINTF_CURRENT_Y*10);
                        KPRINTF_CURRENT_X++;
                        checkwrapping();
                    }
                    break;
                case 'x':
                    char *hexval = "0123456789ABCDEF";
                    uint32_t num  = va_arg(args,uint32_t);
                    for(int sh = 32/4 - 1; sh >= 0; sh--) {
                        char let = hexval[(num>>(sh*4))&0b1111];
                        kprintc(let,KPRINTF_CURRENT_X*8,KPRINTF_CURRENT_Y*10);
                        KPRINTF_CURRENT_X++;
                        checkwrapping();
                    }
                    break;
                case 'd':
                    int n = va_arg(args,int);
                    char out[20];
                    kitoa(n,out,10);
                    
                    for(int j=0;out[j]!=0;j++) {
                        kprintc(out[j],KPRINTF_CURRENT_X*8,KPRINTF_CURRENT_Y*10);
                        KPRINTF_CURRENT_X++;
                        checkwrapping();
                    }

                    break;
            }
            in_specifier = false;
            continue;
        }
        else if (current == '%') {
            in_specifier = true;
            if (*(format+i+1)== '\0'){
                kprintc('%',KPRINTF_CURRENT_X*8,KPRINTF_CURRENT_Y*10);
                KPRINTF_CURRENT_X++;
                checkwrapping();
            }
            continue;
        }
        else if (current == '\n') {
            KPRINTF_CURRENT_Y++;
            KPRINTF_CURRENT_X=0;
            checkwrapping(); // filled screen check
            
        }
        else {
            kprintc(current,KPRINTF_CURRENT_X*8,KPRINTF_CURRENT_Y*10);
            KPRINTF_CURRENT_X++;
            checkwrapping();
            
        }
    }
    va_end(args);
    return FILLED_SCREEN;
}
