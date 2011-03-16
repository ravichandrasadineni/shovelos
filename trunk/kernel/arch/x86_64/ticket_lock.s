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

          movw       $1,       %ax      # take a ticket
	lock  xaddw     %ax,      (%rdi)

.ticket_lock_wait_retry:                # wait our turn ?
          pause
          cmpw      %ax,     2(%rdi)
          jne .ticket_lock_wait_retry

.ticket_lock_aquired:

          # store the lock holders interrupt state
          movb       $0,      4(%rdi)
          pushfq
          testq  (%rsp),      $0x200
          cmovnzb    $1,      4(%rdi)
          addq       $8, %rsp

		  cli
		  retq


######################################################################
# ticket_lock_signal
#   release a fifo spin-lock
#   void ticket_lock_signal( struct ticket_lock * ticket_lock)
######################################################################

.global ticket_lock_signal
ticket_lock_signal:

         xorb       %al,   %al
         cmpb        $0, 4(%rdi)
         cmovnzb     $1,   %al

.ticket_lock_signal_return:

    lock incw 2(%rdi)

.ticket_lock_released:

         cmpb        $0,   %al

         je .ticket_lock_return

         sti

.ticket_lock_return:
         retq

