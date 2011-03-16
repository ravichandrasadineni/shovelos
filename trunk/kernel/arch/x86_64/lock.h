/*
 * lock.h
 *
 *  Created on: Jan 21, 2011
 *      Author: cds
 */

#ifndef LOCK_H_
#define LOCK_H_

#include <inttypes.h>

typedef uint8_t lock_t;

void spinlock_wait(volatile lock_t* lock);

static inline void spinlock_signal(volatile lock_t *lock) {

	__asm__ __volatile__ ( "lock decl 4(%%rdi);" );
}

#endif /* LOCK_H_ */

