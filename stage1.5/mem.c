
#include "16bitreal.h"
#include "mem.h"

void *memcpy(void *dst,const void* src, int size) {

	while(size--)
        ((char*)dst)[size] = ((char*)src)[size];

	return dst;
}

void *memset(void* dst, int c, int size) {
	while(size--)
		((char*)dst)[size] = (char)c;

	return dst;
}

