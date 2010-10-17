

#include "16bitreal.h"
#include "alloc.h"

extern short _heap_start;

void *alloc(unsigned short size) {

	void *ret = (void*)(int)_heap_start;

	_heap_start += size;

	return ret;
}

void *zalloc(unsigned short size) {

	void *ret = alloc(size);

	memset(ret,0,size);

	return ret;
}

void zalloc_align(unsigned short boundary, unsigned short size) {

	// move heap start to next boundary.
	if(alloc(0) % boundry)
		alloc(boundry - (alloc(0) % boundry));

	return zalloc(size);
}