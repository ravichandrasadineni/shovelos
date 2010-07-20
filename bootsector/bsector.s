####################################################################################
#                         SHOVEL OS i386 BOOT SECTOR                               #
#                                                                                  #
#    AUTHOR:  Chris Stones ( chris.stones _AT_ gmail.com )                         #
#    CREATED: 17th JULY 2010                                                       #
#                                                                                  #
####################################################################################

.code16
.arch i386
.section .text


###################################################
# MEMORY MAP                                      #
#                                                 #
#    CODE:                                        #
#         0000:7c00 -> 0000:7dbe                  #
#                                                 #
#    PARTITION TABLE:                             #
#         0000:7dbe -> 0000:7dfe                  #
#                                                 #
#    STACK:                                       #
#         17e0:ffff -> 17e0:0000                  #
#                                                 #
#    DATA:                                        #
#         07e0:0000 -> 07e0:ffff                  #
#                                                 #
#    DATA: Extended Drive Parameters              #
#         07e0:0000 -> 07e0:01eh                  #
#                                                 #
#    DATA: Disk Address Packet                    #
#         07e0:01eh -> 07e0:01fh                  #
#                                                 #
#                                                 #


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


###############################################################
#                   ENTRY POINT ( 0x7C00 )                    #
###############################################################
.global main
main:
  jmp do_main
 
magick:
  .string "ShovelBS"

do_main:

  ##############################################################
  # setup segment registers.
  # code starts at 0x0000:0x7c00 and has size 512 bytes
  # data starts after code at 0x07e0:0x0000 and grows up (64k)
  # stack starts at 0x17e0:0xffff and grows down. (64k)
  ##############################################################
  movw $0x17e0, %ax
  movw %ax,     %ss
  movw $0xffff, %sp
  movw $0x07e0, %ax
  movw %ax,     %ds

  
  
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
#    Parameter ah = character to print
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

  xorl %ebx, %ebx	# clear ebx ( cant use indirect base,index,scale with 16bit regs? )
  xorl %esi, %esi	# clear ecx ( cant use indirect base,index,scale with 16bit regs? )

  movw $nums,         %si           # hex number string array
  movw $gnumstring,	  %di           # output string
  addw $0x0005,		  %di           # seek to end
  movw $0x0004,		  %cx           # loop counter
putn_loop:
       movw           %ax, 	%bx	    # number to bx
       sar        $0x0004,	%ax	    # number >>= 4
       and        $0x000f, 	%bx	    # bx &= 0xf
  cs   movb (%esi,%ebx,1),  %dh     # dh = nums[bx]
  cs   movb           %dh,	(%di)	# *output = dh
       dec            %di           # --output
       dec            %cx           # dec loop counter
       jnz      putn_loop           # loop while not zero
       movw   $gnumstring,  %si     # print buffer
       call          puts
       ret

#####################################################################################
#  puts
#    put string
#    parameters %si: string to print
#####################################################################################
puts:
  cld               # Clear direction flag
puts_loop:
  cs   lodsb        # load byte from cs:si to al, postincrement si
  or   %al, %al     # if terminating null ?
  jz   puts_end     # then finish
  call putc         # put character
  jmp  puts_loop    # next
puts_end:
  ret

#####################################################################################
# die_without_edd
#   check for BIOS Enhanced Disk Drive Services. Quit if not present
#####################################################################################
die_without_edd:
  movb $0x80,   %dl # select first hard drive
  movb $0x41,   %ah # are extensions available
  movw $0x55aa, %bx # no idea?  http://en.wikipedia.org/wiki/INT_13
  int  $0x0013      # call bios
  cmpw $0x0007,	%cx # require all flags set
  jne die           # otherwise die
  ret

###########################################
# read_drive_params
#  Extended Read Drive Parameters.
#  writes 07e0:0000 -> 07e0:01eh.
###########################################
read_drive_params:
  movw $0x00, %si    # destination address ds:si
  movb $0x48, %ah    # INT 13h FUNCTION 48h
  movb $0x80, %dl    # Drive Number
			         # ds:si set by caller
  int  $0x0013       # call BIOS
  jc   die           # CF set on error
  ret

################################################################################
#  static data ( in code segment )
################################################################################
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

