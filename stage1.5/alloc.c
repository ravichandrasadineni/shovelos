

#include "16bitreal.h"
#include "alloc.h"

void *malloc(unsigned short size) {

	void *ret = (void*)_heap_start;

	*((unsigned short *)_heap_start) += size;

	return ret;
}

