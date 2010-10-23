

#include "16bitreal.h"
#include "alloc.h"
#include "mem.h"

extern short _heap_start;

void *alloc(unsigned short size) {

	void *ret = (void*)(int)_heap_start;

	printf("alloc %d (0x%x)\n", size, ret);

	_heap_start += size;

	return (int)ret & 0xffff;
}

void *zalloc(unsigned short size) {

	void *ret = alloc(size);

	memset(ret,0,size);

	return ret;
}

void *zalloc_align(unsigned short boundary, unsigned short size) {

	// move heap start to next boundary.
	if((int)alloc(0) % boundary)
		alloc(boundary - ((unsigned int)alloc(0) % boundary));

	return zalloc(size);
}
