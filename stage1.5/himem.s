

########################################################################################
# high memory access
# * enter long mode
# * do work
# * return to real mode ( or jump to stage 2 )
########################################################################################

.code16

.global himem
himem:

  pusha										# store current state

  mov $0xa0, %eax							# Set PAE and PGE
  mov %eax,  %cr4

  movl $_pml4e, %edx						# Load page tables
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

.code64
												# fake a far-return frame
  push $16										# push compataility mode code selector
  xorq %rcx,%rcx
  movw $compat_mode, %cx
  push %rcx										# push return address
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
  popa
  ret

