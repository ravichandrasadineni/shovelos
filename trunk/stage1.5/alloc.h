

#ifndef __ALLOC_H
#define __ALLOC_H

void *alloc(unsigned short size);
void *zalloc(unsigned short size);
void *zalloc_align(unsigned short boundary, unsigned short size);


#endif /*** __ALLOC_H ***/

