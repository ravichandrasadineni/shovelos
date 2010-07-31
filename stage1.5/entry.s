
.code16gcc

#############################################
# setup segments, and allocate stack.
#   c code assumes all segments are equal.
#############################################
.global main
main:
    movw $0x07e0, %ax
    movw     %ax, %ds
    movw     %ax, %ss
    movw     %ax, %es
    movw $0xffff, %sp
    ljmp $0x07e0, $cmain   # all c code will run in segment 0x07e0



