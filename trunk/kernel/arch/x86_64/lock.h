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

void spinlock_signal(volatile lock_t *lock);

#endif /* LOCK_H_ */

