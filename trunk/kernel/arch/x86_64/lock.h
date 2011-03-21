/*
 * lock.h
 *
 *  Created on: March 18, 2011
 *      Author: cds
 */

#ifndef __X86_64_LOCK_H
#define __X86_64_LOCK_H

#include <inttypes.h>
#include "rflags.h"

struct spin_lock {

	unsigned lock     : 1;
	unsigned rflag_if : 1;
};

#define SPIN_LOCK(name) struct spin_lock name = { 0, 0 }

void spin_lock_wait( struct spin_lock * spin_lock);

void spin_lock_signal( struct spin_lock * spin_lock);

#endif /*** __X86_64_LOCK_H ***/

