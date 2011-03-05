
#ifndef __MEM_H
#define __MEM_H

#define DISK_BUFFER ((uint8_t*)0x20000)
#define ADHOC_COMM  ((uint8_t*)0x30000)
#define MB_MMAP     ((uint8_t*)0x40000) // ALSO in himem.s ( parameter 1 to kernel )

void *memcpy(void *dst,const void* src, int size);

void *memset(void* dst, int c, int s);

int strcmp(const char *s1, const char *s2);

int strlen(const void* s1);

int mkaddr20(int seg, void* offset);

int addds(void* offset);
int addes(void* offset);



#endif /*** __MEM_H ***/

