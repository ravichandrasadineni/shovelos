
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
    ljmp $0x0000, $cmain



//	movl $0x2401, %eax        # enable a20
//	int $0x15

