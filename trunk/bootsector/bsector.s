
.code16
.section .text
.global main
main:
  xorw %ax,   %ax
  movw %ax,   %ds	# zero data  segment
  movw %ax,   %ss	# zero stack segment
  xorl %esp,  %esp	# zero esp
  movw  $0x9c00, %sp	# stack top after bootsector
  call die_without_edd	# check bios support for disk io
  


  movw $msg_string,%si
  call puts
  movw $alive_string,%si
  call puts
  
  jmp halt

#####################################################################################
#  die
#    somthing went wrong. notify user and loop forever.
#####################################################################################
die:
  movw $msg_string,%si
  call puts
  movw $dead_string,%si
  call puts
halt:
  jmp halt

#####################################################################################
#  putc
#    put char
#####################################################################################
putc:
  movb $0x0E, %ah
  int  $0x10
  ret

#####################################################################################
#  putn
#    put number
#    parameters ax
#####################################################################################
putn:

  xorl %ebx, 		%ebx	# clear ebx ( cant use indirect base,index,scale with 16bit regs? )
  xorl %esi, 		%esi	# clear ecx ( cant use indirect base,index,scale with 16bit regs? )

  movw $nums,		%si	# hex number string array
  movw $gnumstring,	%di	# output string
  addw $0x0005,		%di	# seek to end
  movw $0x0004,		%cx	# loop counter
putn_loop:
  movw     %ax, 	%bx	# number to ax
  sar  $0x0004, 	%ax	# number >>= 4
  and  $0x000f, 	%bx	# bx &= 0xf
  movb (%esi,%ebx,1),	%dh	# dh = nums[bx]
  movb  %dh,		(%di)	# *output = dh
  dec  %di			# --output
  dec  %cx			# dec loop counter
  jnz putn_loop			# loop while not zero

  movw $gnumstring, %si		# print buffer
  call puts

  ret

#####################################################################################
#  puts
#    put string
#    parameters %si: string to print
#####################################################################################
puts:
  cld
pust_loop:
  lodsb			
  or %al, %al
  jz puts_end
  call putc
  jmp  pust_loop
puts_end:
  ret

#####################################################################################
# die_without_edd
#   check for BIOS Enhanced Disk Drive Services. Quit if not present
#####################################################################################
die_without_edd:
  movb $0x80, 	%dl	# select first hard drive
  movb $0x41, 	%ah	# are extensions available
  movw $0x55aa, %bx	# no idea?  http://en.wikipedia.org/wiki/INT_13
  int  $0x0013		# call bios

  cmpl $0x0007, %cx	# require all flags set
  jne die		# otherwise die

  ret

#### PRINT RETURN VALUES ####
#  pushw %bx		# store bx (putn will clobber)
#  pushw %cx		# store cx (putn will clobber)
#  movb   %ah, %al
#  xorb   %ah, %ah
#  call putn
#  movw  2(%esp), %ax
#  call putn
#  movw  0(%esp), %ax
#  call putn
#  add $0x0004, %sp
#  ret
############################
  
msg_string:
  .asciz "ShovelOS bootsector is "
dead_string:
  .asciz "DEAD!\r\n"
alive_string:
  .asciz "ALIVE!\r\n"
nums:
  .asciz "0123456789abcdef"
gnumstring:
  .asciz "0x????\r\n"

