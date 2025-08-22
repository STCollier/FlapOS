#ifndef UTIL_H
#define UTIL_H

#define uint8_t  unsigned char
#define uint16_t unsigned short
#define uint32_t unsigned int
#define uint64_t unsigned long
typedef uint32_t size_t;

#define true 1
#define false 0

static inline void *memset(void *s, int c, size_t n) {
	const unsigned char uc = c;
	unsigned char *su = (unsigned char *)s;

	for (; 0 < n; ++su, --n)
		*su = uc;

	return (s);
}

static inline void outportb(uint16_t port, uint8_t data) {
    asm("outb %1, %0" : : "dN" (port), "a" (data));
}

#endif