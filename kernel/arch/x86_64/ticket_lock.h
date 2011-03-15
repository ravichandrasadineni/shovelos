/*
 * ticket_lock.h
 *
 *  Created on: March 12, 2011
 *      Author: cds
 */

#ifndef __X86_64_TICKET_LOCK_H
#define __X86_64_TICKET_LOCK_H

#include <inttypes.h>
#include "lock.h"

typedef uint16_t ticket_t;
typedef uint16_t lock_t;

struct ticket_lock {

	lock_t queue_lock;
	lock_t dequeue_lock;
	ticket_t queue_ticket;
	ticket_t dequeue_ticket;
};

void ticket_lock_wait( volatile struct ticket_lock * ticket_lock);
void ticket_lock_signal( volatile struct ticket_lock * ticket_lock);


#endif /*** __X86_64_TICKET_LOCK_H ***/

