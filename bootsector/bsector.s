####################################################################################
#	                          SHOVEL OS BOOT SECTOR
#
#    AUTHOR:  Chris Stones ( chris.stones _AT_ gmail.com )
#    CREATED: 17th JULY 2010
#
####################################################################################

.code16
.section .text

###################################################
# IBM PC - 16 byte partition record.              #
###################################################
# OFF  # SIZE #             DESCR                 #
###################################################
# 0x00 # 0x01 # BOOTABLE(0x80) or 0x00            #
# 0x01 # 0x03 # CHS of first absolute sector      #
# 0x01 # 0x01 # ^ HE7,HE6,HE5,HE4,HE3,HE2,HE1,HE0 #
# 0x02 # 0x01 # ^ CY9,CY8,SE5,SE4,SE3,SE2,SE1,SE0 #
# 0x03 # 0x01 # ^ CY7,CY6,CY5,CY4,CY3,CY2,CY1,CY0 #
# 0x04 # 0x01 # partition type                    #
# 0x05 # 0x03 # CHZ of last absolute sector       #
# 0x05 # 0x01 # ^ HE7,HE6,HE5,HE4,HE3,HE2,HE1,HE0 #
# 0x06 # 0x01 # ^ CY9,CY8,SE5,SE4,SE3,SE2,SE1,SE0 #
# 0x07 # 0x01 # ^ CY7,CY6,CY5,CY4,CY3,CY2,CY1,CY0 #
# 0x08 # 0x04 # LBA of first absolute sector      #
# 0x0C # 0x04 # number of sectors (little-endian) #
###################################################


#####################################################################################
#                            ENTRY POINT ( 0x7C00 )
#####################################################################################
.global main
main:
  jmp domain
 
magick:
  .string "ShovelBS"
  
boot_partition:
.short 0x00

domain:
  xorw %ax,   %ax
  movw %ax,   %ds			# zero data  segment
  movw %ax,   %ss			# zero stack segment
  xorl %esp,  %esp		# zero esp
  movw  $0x9c00, %sp		# stack top after bootsector
  call die_without_edd		# check bios support for disk io
  
  
  movw $msg_string,%si		# PRINT ALIVE MESSAGE
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

  movw $nums,		%si		# hex number string array
  movw $gnumstring,	%di		# output string
  addw $0x0005,		%di		# seek to end
  movw $0x0004,		%cx		# loop counter
putn_loop:
  movw     %ax, 		%bx		# number to ax
  sar  $0x0004,		%ax		# number >>= 4
  and  $0x000f, 		%bx		# bx &= 0xf
  movb (%esi,%ebx,1),	%dh		# dh = nums[bx]
  movb  %dh,		(%di)		# *output = dh
  dec  %di					# --output
  dec  %cx					# dec loop counter
  jnz putn_loop				# loop while not zero
  movw $gnumstring,	%si		# print buffer
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
  movb $0x41,	%ah	# are extensions available
  movw $0x55aa,	%bx	# no idea?  http://en.wikipedia.org/wiki/INT_13
  int  $0x0013		# call bios
  cmpl $0x0007,	%cx	# require all flags set
  jne die			# otherwise die

  ret


###########################################
# read_drive_params
#  Extended Read Drive Parameters
#  Parameters ds:si - output buffer
#  Returns BIOS return code in reg ah
###########################################
read_drive_params:
  movb $0x48, %ah	# INT 13h FUNCTION
  movb $0x80, %dl	# Drive Number
			# ds:si set by caller
  int  $0x0013	# call BIOS
  jc   die		# CF set on error
  ret


  
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

