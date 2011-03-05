/*
 * mem.c
 *
 *  Created on: Mar 5, 2011
 *      Author: cds
 */

#include <inttypes.h>
#include <mem.h>

void *memmove(void* _dst, const void* _src, sint32_t size) {

  int dst = (int)_dst;
  int src = (int)_src;

  if((dst>dst ? dst-src : src-dst)>=size) {
    return memcpy((void*)dst,(void*)src,size);
  }
  else if(dst>src) {
    memcpy((void*)(src+size),(void*)(dst),(src+size)-dst);
    memcpy((void*)dst,(void*)src,dst-src);
  }
  else
  {
    memcpy((void*)dst,(void*)src,src-dst);
    memcpy((void*)src,(void*)(src+(src-dst)),(dst+size) - src);
  }
  return _dst;
}

