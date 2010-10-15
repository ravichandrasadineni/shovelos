

#include "16bitreal.h"
#include "alloc.h"

extern short _heap_start;

void *alloc(unsigned short size) {

	void *ret = (void*)(int)_heap_start;

	_heap_start += size;

	return ret;
}

