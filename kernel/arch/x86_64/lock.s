/*
 * lock.s
 *
 *  Created on: Mar 12, 2011
 *      Author: cds
 */

.text

.global spinlock_wait
spinlock_wait:
.retry:
	movb $1, %al
    xchg %al,(%rdi)
    test $1, %al
    jz .ret
.read_loop:
    movb (%rdi), %al
    test $1, %al
    jz   .retry
    jmp  .read_loop
.ret:
	retq

