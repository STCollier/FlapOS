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

#define low_16(address) (uint16_t)((address) & 0xFFFF)
#define high_16(address) (uint16_t)(((address) >> 16) & 0xFFFF)

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wbuiltin-declaration-mismatch"

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

static inline void outportb(uint16_t port, uint8_t data) {
    asm("outb %1, %0" : : "dN" (port), "a" (data));
}

static inline uint8_t inb(uint16_t port) {
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

#endif
