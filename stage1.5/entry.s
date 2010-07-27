
.code16gcc

#############################################
# zero segments, and allocate stack.
#   c code assumes all segments are zero.
#############################################
.global main
main:
    xorw     %ax, %ax
    movw     %ax, %cs
    movw     %ax, %ds
    movw     %ax, %ss
    movw     %ax, %es
    movw $0xffff, %sp
    jmp    cmain


