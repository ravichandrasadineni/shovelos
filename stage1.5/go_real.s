
##############################################################################
# return to real mode from long mode
##############################################################################

.code64

.global exit_long_mode
exit_long_mode:

  #FIRST, return to compatability mode
  push $8
  xorq %rcx,%rcx
  mov $compat_mode, %ecx
  push %rcx
  retf
compat_mode:

  # clear segments, restore real stack
  xor %eax,%eax
  mov %eax,%ss
  mov %eax,%ds
  mov %eax,%es
  mov %eax,%gs
  mov %eax,%fs
  mov real_stack, %esp

  #Disable Paging and protection
    movq %cr0, %rax
    andq $0x7FFFFFFE, %rax
    movq %rax, %cr0

  #Deactivate Long Mode
    movl $0xc0000080, %ecx # EFER MSR number.
    rdmsr
    btc $8, %eax
    wrmsr

.code16
  # Jump back to realmode code
    jmp return_from_long_mode

