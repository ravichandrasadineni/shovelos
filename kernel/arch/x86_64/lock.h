/*
 * lock.h
 *
 *  Created on: Jan 21, 2011
 *      Author: cds
 */

#ifndef LOCK_H_
#define LOCK_H_

#include <inttypes.h>

inline uint8_t lock_try(uint8_t *lock) {

   uint8_t ret;
   __asm__ __volatile__( "movb  $1,  %%al;"
		                 "xchg  %%al,(%1);"
		                :"=a" (ret)
		                :"r"  (lock)   );
   return ret;
}

inline void lock_release(uint8_t *mem) {

	*mem = 0;
}

inline void lock_spin(uint8_t* lock) {

	while( lock_try(lock) != 0 );
}

#endif /* LOCK_H_ */

