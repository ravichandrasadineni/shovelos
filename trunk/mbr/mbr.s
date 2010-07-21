####################################################################################
#                         SHOVEL OS i386 MBR                                       #
#                                                                                  #
#    AUTHOR:  Chris Stones ( chris.stones _AT_ gmail.com )                         #
#    CREATED: 17th JULY 2010                                                       #
#                                                                                  #
#    1) Relocate to top of 16bit address space                                     #
#    2) Parse partition table and chainload VBR                                    #
#                                                                                  #
####################################################################################

.code16
.arch i386
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


###############################################################
#                   ENTRY POINT ( 0000:0x7C00 )               #
###############################################################
.global main
main:
  ljmp $0x0000, $relocate  # jump over config table.
                           # grub warns of buggy bios
                           # versions that enter at 0x07c0:000
                           # so we use a far jump here.

###############################################################
#                         CONFIG DATA                         #
###############################################################
magick:
  .string "ShovMBR"

override_active_partition:
  .value 0xff

###############################################################
#  GET OUT OF THE WAY OF THE VBR                              #
#    -  RELOCATE TO 1000:7c00                                 #
###############################################################
relocate:

  # relocate from 0000:7c00 to 0x1000:7c00
  movw %cs,     %ax        # from segment
  movw %ax,     %ds
  movw $0x7c00, %si        # from offset
  movw $0x1000, %ax
  movw %ax,     %es        # to segment
  movw $0x7c00, %di        # to offset
  movw $0x80,   %cx        # loop counter
  rep                      # repeat
  movsd                    # move quadword
  ljmp $0x1000, $relocated_main

###############################################################
#  RELOCATED CODE BEGINS HERE                                 #
#    NOW RUNNING IN CS=0x1000                                 #
###############################################################
relocated_main:

  movw $0x2000, %ax
  movw %ax,     %ds        # 64k of data  at segment 0x2000
  movw $0x3000, %ss        # 64k of stack at segment 0x3000
  movw $0xffff, %sp
  movw %ax,     %ax        # partition 0
  movw $0x7dbe, %bx        # address of first partition table

try_partition:  
  cmpw  $0x80, (%bx)       # this partition marked as active?
  je    found_active       # boot it
  cmpw  $0x03, %ax         # no primary partitions active?
  je    none_active        # quit!
  incw  %ax                # next partition number
  addw $0x0010, %bx        # next partition table
  jmp  try_partition       # re-try

none_active:

  movw

found_active:

  jmp halt


#####################################################################################
#  read_disk_sectors
#    read sectors from disk
#####################################################################################
read_disk_sectors:
  movw   $0x001e, %si     # Disk Access Packet
  movb   $0x10,   0(%si)  # Packet size
  movb   $0x00,   1(%si)  # Unused
  movb   $0x01,   2(%si)  # sectors to read
  movb   $0x00,   3(%si)  # Unused
  movw   $0x002e, 4(%si)  # load to offset
  movw   %ds,     %ax
  movw   %ax,     6(%si)  # load to segment
  movl   $0x00,   8(%si)  # first sector to load
  movl   $0x00,  12(%si)  # first sector to load

  movb $0x42, %ah         # BIOS function for extended read
  movb $0x80, %dl         # first hard disk
                          # si already loaded with packet offset
  int $0x13
  ret
  

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

  movw $nums,         %si             # hex number string array
  movw $gnumstring,   %di             # output string
  addw $0x0005,       %di             # seek to end
  movw $0x0004,       %cx             # loop counter
putn_loop:
       movw           %ax,      %bx   # number to bx
       sar        $0x0004,      %ax   # number >>= 4
       and        $0x000f,      %bx   # bx &= 0xf
  cs   movb (%esi,%ebx,1),      %dh   # dh = nums[bx]
  cs   movb           %dh,     (%di)  # *output = dh
       dec            %di             # --output
       dec            %cx             # dec loop counter
       jnz      putn_loop             # loop while not zero
       movw   $gnumstring,      %si   # print buffer
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
non_active_string:
  .asciz "couldn't find any active partitions to boot."
