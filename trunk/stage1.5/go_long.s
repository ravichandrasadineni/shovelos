
########################################################################################
# Enter long mode directly from real mode ( no temporary protected mode required )
# Thanks to Brendan of http://forum.osdev.org/viewtopic.php?t=11093
########################################################################################

.code16

.global enter_long_mode
.global return_from_long_mode
.global real_stack

real_stack:
  .long 0

enter_long_mode:

  # store current state
  pusha
  movl %esp, real_stack

  # Set PAE and PGE
  mov $0xa0, %eax
  mov %eax,  %cr4

  # Load page tables
  movl g_pmle4, %edx
  movl %edx,    %cr3

  # Enable long mode
  mov $0xC0000080, %ecx
  rdmsr
  or $0x00000100, %eax
  wrmsr

  # Activate Compatability Mode ( enable paging and protection )
  movl %cr0,%ebx
  orl  $0x80000001, %ebx
  movl %ebx, %cr0

  # Load GDT
  movl gdt_reg, %eax
  lgdt (%eax)

  # jump to long mode ( exits compatability mode )
  jmp $8, $long_main

return_from_long_mode:

  # restore last real state
  popa
  ret

long_main:
  cli
  long_loop:
  jmp   long_loop

