
#ifndef __MEM_H
#define __MEM_H

void *memcpy(void *dst,const void* src, int size);
void *memcpy_far(void *dst,const void* src, int size);


void *memset(void* dst, int c, int s);

int strcmp(const char *s1, const char *s2);

#endif /*** __MEM_H ***/

