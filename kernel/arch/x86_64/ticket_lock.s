/*
 * ticket_lock.s
 *
 *  Created on: Mar 16, 2011
 *      Author: cds
 */

.text

######################################################################
# ticket_lock_wait
#   aquire a fifo spin-lock
#   void ticket_lock_wait( struct ticket_lock * ticket_lock)
######################################################################

.global ticket_lock_wait
ticket_lock_wait:

          movl       $1,       %eax
	lock  xaddl    %eax,      (%rdi)
.ticket_lock_wait_retry:
          pause
          cmpl    %%eax,     4(%rdi);
          jne .ticket_lock_wait_retry;"
		  retq


