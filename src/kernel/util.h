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

#pragma GCC diagnostic pop

static inline void outportb(uint16_t port, uint8_t data) {
    asm("outb %1, %0" : : "dN" (port), "a" (data));
}

#endif