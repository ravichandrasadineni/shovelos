/*
 * isr.c
 *
 *  Created on: 3 Jul 2011
 *      Author: cds
 */


#define INTERRUPT(vector) \
	__asm__(".global x86_64_isr_vector" #vector "\n"\
			"x86_64_isr_vector" #vector ":\n" \
			"    pushq %rax;" \
			"    pushq %rcx;" \
			"    pushq %rdx;" \
			"    pushq %rsi;" \
			"    pushq %rdi;" \
			"    pushq %r8;" \
			"    pushq %r9;" \
			"    pushq %r10;" \
			"    pushq %r11;" \
			"    call x86_64_handle_isr_vector" #vector ";" \
			"    popq %r11;" \
			"    popq %r10;" \
			"    popq %r9;" \
			"    popq %r8;" \
			"    popq %rdi;" \
			"    popq %rsi;" \
			"    popq %rdx;" \
			"    popq %rcx;" \
			"    popq %rax;" \
	"iretq;")

INTERRUPT(0);	// divide by zero
INTERRUPT(1);	// divide by zero
INTERRUPT(2);	// divide by zero
INTERRUPT(3);	// divide by zero
INTERRUPT(4);	// divide by zero
INTERRUPT(5);	// divide by zero
INTERRUPT(6);	// divide by zero
INTERRUPT(7);	// divide by zero
INTERRUPT(8);	// divide by zero
INTERRUPT(9);	// divide by zero
INTERRUPT(10);	// divide by zero
INTERRUPT(11);	// divide by zero
INTERRUPT(12);	// divide by zero
INTERRUPT(13);	// divide by zero
INTERRUPT(14);	// divide by zero
INTERRUPT(15);	// divide by zero
INTERRUPT(16);	// divide by zero
INTERRUPT(17);	// divide by zero
INTERRUPT(18);	// divide by zero
INTERRUPT(19);	// divide by zero
INTERRUPT(20);	// divide by zero
INTERRUPT(21);	// divide by zero
INTERRUPT(22);	// divide by zero
INTERRUPT(23);	// divide by zero
INTERRUPT(24);	// divide by zero
INTERRUPT(25);	// divide by zero
INTERRUPT(26);	// divide by zero
INTERRUPT(27);	// divide by zero
INTERRUPT(28);	// divide by zero
INTERRUPT(29);	// divide by zero
INTERRUPT(30);	// divide by zero
INTERRUPT(31);	// divide by zero






