
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
/***
 * string compare:
 * Takes two strings.
 * returns non-zero if they differ.
 */
int strcmp(const char *s1, const char *s2) {

	int index,compare;

	for(index=0; s1[index] || s2[index]; index++)
		if((compare = (s1[index] - s2[index])))
			return compare;

	return 0;
}

