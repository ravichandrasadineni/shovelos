

#include "16bitreal.h"
#include "alloc.h"

static char *g_heap_ptr = 0;

void setup_heap(void *heap_start, unsigned short heap_size) {

	g_heap_ptr = heap_start;

	memset(g_heap_ptr, 0, heap_size);

	*((unsigned short*)g_heap_ptr) = heap_size;
}

void *malloc(unsigned short size) {

	short oldsize = 0;
	void *ret = (void*)g_heap_ptr;

	if(g_heap_ptr)
		oldsize = *((unsigned short *)g_heap_ptr);

	if(!g_heap_ptr || (oldsize < size))
		halt("out of heap");

	if(*g_heap_ptr >= (size+2)) {
		g_heap_ptr += size;
		*((unsigned short *)g_heap_ptr) = oldsize-size;
	}
	else {
		// not enough for size header after split, mark as out of heap
		g_heap_ptr = 0;
	}

	return ret;
}

