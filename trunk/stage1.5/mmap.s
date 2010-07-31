
.code16gcc

#
# read memory map from BIOS ( use int 15h EAX=e820h )
#

################################################################################################
### http://wiki.osdev.org/Detecting_Memory_(x86)#BIOS_Function:_INT_0x15.2C_EAX_.3D_0xE820   ###
################################################################################################
# First qword = Base address
# Second qword = Length of "region" (if this value is 0, ignore the entry)
# Next dword = Region "type"
# Type 1: Usable (normal) RAM
# Type 2: Reserved - unusable
# Type 3: ACPI reclaimable memory
# Type 4: ACPI NVS memory
# Type 5: Area containing bad memory
#
# Next dword = ACPI 3.0 Extended Attributes bitfield (if 24 bytes are returned, instead of 20)
# Bit 0 of the Extended Attributes indicates if the entire entry should be ignored (if the bit is clear). This is going to be a huge compatibility problem because most current OSs won't read this bit and won't ignore the entry.
# Bit 1 of the Extended Attributes indicates if the entry is non-volatile (if the bit is set) or not. The standard states that "Memory reported as non-volatile may require characterization to determine its suitability for use as conventional RAM."
# The remaining 30 bits of the Extended Attributes are currently undefined.


###############################
# write memory list to 2(_heap_start)
# write list length to 0(_heap_start)
# set _mmap to _heap_start
# adjust _heap_start
###############################
.global _16_read_memory_map
_16_read_memory_map:
  push %bp
  movw %sp, %bp
  push %ebx

  movw $_heap_start, %di
  addw $0x0002,      %di      # array destination
  xorl %ebx,         %ebx     # clear ebx

.rmm_next:
  movl $0x534D4150, %edx      # call bios
  movl $0x0000e820, %eax
  movl $0x00000018, %ecx
  int  $0x00000015

  jc   .rmm_exit
  cmpl $0x534d4150, %eax
  jne  .rmm_exit

  cmpb $0x14, %cl
  jne  .rmm_continue

.rmm_clear_last_dword:
  movl $0, 20(%di)            

.rmm_continue:
  addw $0x0018, %di           # inc array address
#  jmp .rmm_next

.rmm_exit:
  pop %ebx
  pop %bp
  ret


