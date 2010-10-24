
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
  push rcx
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
    movl %cr0, %eax
    andl $0x7FFFFFFE, %eax
    mov %eax, %cr0

  #Deactivate Long Mode
    movl $0xc0000080, %ecx # EFER MSR number.
    rdmsr
    btc $8, %eax
    wrmsr

  # Jump back to realmode code
    ljmp $0, $return_from_long_mode

