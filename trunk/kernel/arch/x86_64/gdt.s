#
# gdt.s
#
#  Created on: Dec 29, 2010
#      Author: cds
#

.text

######
# load a new GDT
#  parameter 1: virtual address of gdtr
#  parameter 2: new code descriptor offset
#  parameter 3: new data descriptor offset
.global _x86_64_asm_lgdt
_x86_64_asm_lgdt:

    lgdt (%rdi)
    movq %rdx, %ds
    movq %rdx, %ss
    movq %rdx, %es
    movq %rdx, %fs
    movq %rdx, %gs

    pushq %rsi										# push code selector
    movabsq $.done, %r10
    pushq %r10										# push return address
    retfq											# far-return to new cs descriptor
.done:
    retq
