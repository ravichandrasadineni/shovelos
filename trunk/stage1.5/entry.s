
.code16gcc

#############################################
# setup segments, and allocate stack.
#   c code assumes all segments are equal.
#############################################
.global main
main:
    cli
    movw $0x0000, %ax
    movw     %ax, %ds
    movw     %ax, %ss
    movw     %ax, %es
    movw $0xffff, %sp

#############################################
# Enable A20 Line
#############################################
    movl $0x2401, %eax
    int $0x15

#############################################
# Jump to c main
#############################################
    ljmp $0x0000, $cmain

#############################################
# HALT EXECUTION
#############################################
.globl halt
halt:
    add $4, %esp
	call	puts
.halt_loop:
	jmp .halt_loop
