
.PHONY: all clean
all : stage1.5.bin

# 64bit assembler source
SRC64_S = go_real.s

# 32bit assembler source
SRC32_S = entry.s \
		go_long.s
		
# 32bit c source
SRC32_C = main.c \
		  print.c \
	      mmap.c \
		  a20.c \
		  alloc.c \
		  mem.c \
		  math64.c \
		  bios_disk.c \
		  pt.c \
		  gdt.c 

OBJ64=$(SRC64_S:.s=.64.o) $(SRC64_C:.c=.64.o)
OBJ32=$(SRC32_S:.s=.32.o) $(SRC32_C:.c=.32.o)

# Convert an elf to a flat
%.bin : %.elf
	objcopy -O binary $< $@

# Link real and long mode objects into an elf.
stage1.5.elf : $(OBJ64) $(OBJ32) link.script
	$(LD) -s -Map link.map -static -nodefaultlibs -nostdlib --nmagic -Tlink.script -o stage1.5.elf $(OBJ64) $(OBJ32)
	
%.32.o : %.c 
	gcc -c -std=c99 -m32 -Os -fomit-frame-pointer -fno-unroll-loops -ffreestanding -nostdinc -o $@ $<
	objcopy -Felf64-x86-64 $@
	
%.64.o : %.c
	gcc -c -std=c99 -m64 -Os -fomit-frame-pointer -fno-unroll-loops -ffreestanding -nostdinc -o $@ $<
#	objcopy -Felf32-i386 $@

%.32.o : %.s
	gcc -c -m32 -ffreestanding -o $@ $<
	objcopy -Felf64-x86-64 $@
	
%.64.o : %.s
	gcc -c -m64 -ffreestanding -o $@ $<
#	objcopy -Felf32-i386 $@
	
clean:
	rm -f *~
	rm -f *.o
	rm -f stage1.5.elf
	rm -f stage1.5.bin
	
	
