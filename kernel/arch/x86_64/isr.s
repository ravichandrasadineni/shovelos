#
# isr.s
#
#  Created on: Dec 31, 2010
#      Author: cds
#

.global dummy_isr
dummy_isr:



 iretq

# divide by zero
.global _x86_64_isr_vector0_divide_error
_x86_64_isr_vector0_divide_error:
	pushq %rax
#	pushq %rbx        # preserve
	pushq %rcx
	pushq %rdx
	pushq %rsi
	pushq %rdi
#	pushq %rbp        # preserve
#	pushq %rsp        # processor pushes return rsp
	pushq %rsp
	pushq %r8
	pushq %r9
	pushq %r10
	pushq %r11
#	pushq %r12        # preserve
#	pushq %r13        # preserve
#	pushq %r14        # preserve
#	pushq %r15        # preserve

	call x86_64_handle_divide_error

#	popq %r15        # preserve
#	popq %r14        # preserve
#	popq %r13        # preserve
#	popq %r12        # preserve
	popq %r11
	popq %r10
	popq %r9
	popq %r8
#	popq %rsp        # processor pushes return rsp
#	popq %rbp        # preserve
	popq %rdi
	popq %rsi
	popq %rdx
	popq %rcx
#	popq %rbx        # preserve
	popq %rax
	iretq
