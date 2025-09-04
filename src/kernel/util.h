#ifndef UTIL_H
#define UTIL_H

#define uint8_t  unsigned char
#define uint16_t unsigned short
#define uint32_t unsigned int
#define uint64_t unsigned long
typedef uint32_t size_t;
typedef uint32_t uintptr_t;
#define true 1
#define false 0
#define bool uint8_t
#define outb outportb
#define inb  inportb

typedef struct {
    int x, y;
} vec2_t;

typedef struct {
    float x,y;
} fvec2_t;

#define FVEC2_ZERO (fvec2_t){0 , 0}
#define VEC2_ZERO (vec2_t) {0, 0}
#define ARR_LEN(arr) (int) (sizeof(PIPES) / sizeof(PIPES[0]))

#define low_16(address) (uint16_t)((address) & 0xFFFF)
#define high_16(address) (uint16_t)(((address) >> 16) & 0xFFFF)

#define TEST_INTERRUPT(x) \
    klog("[DEBUG] Call interrupt %d.", x); \
    __asm__ volatile("int %0" :: "i"(x));

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wbuiltin-declaration-mismatch"
static inline int roundToInt(float dec) {
    if (dec-(int)dec >= 0.5f) return (int)dec+1;
    else return (int)dec;
} 
static inline void *memset(void *s, int c, size_t n) {
	const unsigned char uc = c;
	unsigned char *su = (unsigned char *)s;

	for (; 0 < n; ++su, --n)
		*su = uc;

	return (s);
}

static inline void memcpy(uint8_t *s, uint8_t *d, size_t n) {
	for(;n>0;--n){
		*(s+n)=*(d+n);
	}
}

static inline size_t strlen(const char* str) {
    size_t x = 0;
    while(*str++) x++;
    return x;
}

#pragma GCC diagnostic pop

static const char* itoa(int num, char* str, int base) {
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

    if (neg) str[i++] = '-';

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

static inline void outportb(uint16_t port, uint8_t data) {
    asm("outb %1, %0" : : "dN" (port), "a" (data));
}

static inline uint8_t inportb(uint16_t port) {
    uint8_t ret;
    __asm__ volatile (
		"inb %w1, %b0"
        : "=a"(ret)
        : "Nd"(port)
        : "memory"
	);

    return ret;
}

static inline void io_wait(void) {
    outb(0x80, 0);
}

static inline uint32_t rand(uint32_t seed) {
    static uint32_t x = 123456789;
    static uint32_t y = 362436069;
    static uint32_t z = 521288629;
    static uint32_t w = 88675123;

    x *= 23786259 - seed;

    uint32_t t;

    t = x ^ (x << 11);
    x = y; y = z; z = w;
    return w = w ^ (w >> 19) ^ t ^ (t >> 8);
};

#endif