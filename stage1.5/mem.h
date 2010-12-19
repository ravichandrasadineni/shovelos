
#ifndef __MEM_H
#define __MEM_H

void *memcpy(void *dst,const void* src, int size);

void *memset(void* dst, int c, int s);

int strcmp(const char *s1, const char *s2);

int mkaddr20(int seg, void* offset);

int addds(void* offset);
int addes(void* offset);



#endif /*** __MEM_H ***/

