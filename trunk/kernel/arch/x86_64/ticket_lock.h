/*
 * ticket_lock.h
 *
 *  Created on: March 12, 2011
 *      Author: cds
 */

#ifndef __X86_64_TICKET_LOCK_H
#define __X86_64_TICKET_LOCK_H

#include <inttypes.h>
#include "rflags.h"

struct ticket_lock {

	uint16_t queue;
	uint16_t dequeue;
	uint8_t  rflag_if;
};

#define TICKET_LOCK(name) struct ticket_lock name = { 0, 0 }

void ticket_lock_wait( struct ticket_lock * ticket_lock);

void ticket_lock_signal( struct ticket_lock * ticket_lock);

#endif /*** __X86_64_TICKET_LOCK_H ***/

