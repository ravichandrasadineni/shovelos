/*
 * ticket_lock.s
 *
 *  Created on: Mar 18, 2011
 *      Author: cds
 */

.text

######################################################################
# spin_lock_wait
#   aquire a spin-lock
#   void spin_lock_wait( struct spin_lock * spin_lock)
######################################################################

.global spin_lock_wait
spin_lock_wait:

          movb   $1, %al

.spin_lock_retry:                    # try to aquire the lock.
	lock  xchgb  %al, (%rdi)
	      testb   $1, %al
	      jz .spin_lock_aquired

.spin_lock_read_loop:                # optimisation ?
          pause                      #   read only loop while waiting
          testb $1, (%rdi)            #   for the lock to become free.
          jnz .spin_lock_read_loop   #   no need to lock.
          jmp .spin_lock_retry

.spin_lock_aquired:                  # store callers interrupt state
          pushfq
          testq  $0x200, (%rsp)
          jz .spin_lock_done
          andb $2, (%rdi)
          cli
.spin_lock_done:
          addq       $8, %rsp
		  retq


######################################################################
# spin_lock_signal
#   release a spin-lock
#   void spin_lock_signal( struct spin_lock * spin_lock)
######################################################################

.global spin_lock_signal
spin_lock_signal:

        movb        (%rdi), %al     # read callers old interrupt state

    lock btr         $0, (%rdi)     # release the lock

         testb       $2,   %al      # restore old interrupt state
         jz .ticket_lock_return
         sti

.ticket_lock_return:
         retq


