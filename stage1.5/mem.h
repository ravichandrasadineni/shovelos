
#ifndef __MEM_H
#define __MEM_H

#define PML4E_BUFFER ((uint8_t*)0x10000)
#define MB_MMAP      ((uint8_t*)0x20000) // ALSO in himem.s ( parameter 1 to kernel )
#define ADHOC_COMM   ((uint8_t*)0x30000)
#define ADHOC_COMM_END (ADHOC_COMM + 32)


#define HIGH_HEAP_BEGIN ADHOC_COMM_END
#define HIGH_HEAP_LIMIT 0x80000

#include <inttypes.h>

void *memcpy(void *dst,const void* src, int size);
void *memset(void* dst, int c, int s);
void *memmove(void* _dst, const void* _src, sint32_t size);

int strcmp(const char *s1, const char *s2);

int strlen(const void* s1);

int mkaddr20(int seg, void* offset);

int addds(void* offset);
int addes(void* offset);



#endif /*** __MEM_H ***/

