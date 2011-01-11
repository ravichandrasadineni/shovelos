

########################################################################################
# high memory access
# * enter long mode
# * do work
# * return to real mode ( or jump to stage 2 )
########################################################################################

.code16

.global shuffle_high
shuffle_high:

  pushal										# store current state

  mov $0xa0, %eax							# Set PAE and PGE
  mov %eax,  %cr4

  movl $0x10000, %edx						# Load page tables
  movl %edx,    %cr3

  mov $0xC0000080, %ecx						# Enable long mode
  rdmsr
  or $0x00000100, %eax
  wrmsr

  movl %cr0,%ebx							# Enable paging and protection.
  orl  $0x80000001, %ebx					# to enter compatability mode.
  movl %ebx, %cr0

  movl $_gdt_reg, %eax						# Load GDT
  lgdt (%eax)

  jmp $8, $long_main						# jump to long mode
  											# cs selector index 1 (offset 8)

##############################################
#  LONG MODE BEINS HERE - DO WORK            #
##############################################
.code64
long_main:

  mov $24, %rax									# setup 64bit segments
  mov %rax,%ds
  mov %rax,%ss
  mov %rax,%es
  mov %rax,%gs
  mov %rax,%fs

   movq $0x30000,  %rax
   movq  0(%rax),  %rdi
   movq  8(%rax),  %rsi
   movq 16(%rax),  %rcx

   rep movsb

   cmpq $0, 20(%rax)
   je   exit_long_mode

   movl %esp, %eax
   andl $0xffff, %eax
   subl $8,      %eax
   movq $0xFFFFFFFF80000000, %rcx
   movq %rcx, (%rax)
   jmpq  *(%rax)

##############################################
#  RETURN TO REAL MODE                       #
##############################################
 exit_long_mode:
  mov $24, %eax									# setup 32bit compatability segments
  mov %eax,%ds
  mov %eax,%ss
  mov %eax,%es
  mov %eax,%gs
  mov %eax,%fs
												# fake a far-return frame
  pushq $16										# push compataility mode code selector
  xorq %rcx,%rcx
  movw $compat_mode, %cx
  pushq %rcx										# push return address
  retfq											# far-return to compatability mode
compat_mode:

  .code16
    movl %cr0, %eax								#Disable Paging and protection
    andl $0x7FFFFFFE, %eax
    movl %eax, %cr0

	xor %ax, %ax								# reset segments
	mov %ax, %ds
	mov %ax, %ss
  	mov %ax, %es
  	mov %ax, %gs
  	mov %ax, %fs

    jmp $0, $return_from_long_mode				# and finaly, reset cs
return_from_long_mode:
  popal
  retl

