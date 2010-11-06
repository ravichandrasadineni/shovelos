
.PHONY: all clean
all : stage1.5.ext2.bin

# 32bit assembler source
SRC32_S = entry.s \
		  himem.s
		
# 32bit c source
SRC32_C = main.c \
		  print.c \
          mmap.c \
		  alloc.c \
		  mem.c \
		  bios_disk.c \
		  pt.c \
		  math64.c \
		  fs/ext2/ext2.c

OBJ64=$(SRC64_S:.s=.64.o) $(SRC64_C:.c=.64.o)
OBJ32=$(SRC32_S:.s=.32.o) $(SRC32_C:.c=.32.o)

# Convert an elf to a flat
%.bin : %.elf
	objcopy -O binary $< $@

# Link real and long mode objects into an elf.
stage1.5.ext2.elf : $(OBJ64) $(OBJ32) link.script
	$(LD) -s -Map link.map -static -nodefaultlibs -nostdlib --nmagic --oformat elf32-i386 -melf_i386 -Tlink.script -o $@ $(OBJ64) $(OBJ32)
	
%.32.o : %.c 
	gcc -c -DDEBUG=1 -std=c99 -m32 -Os -fomit-frame-pointer -fno-unroll-loops -ffreestanding -nostdinc -o $@ $<
	
%.64.o : %.c
	gcc -c -std=c99 -m64 -Os -fomit-frame-pointer -fno-unroll-loops -ffreestanding -nostdinc -o $@ $<

%.32.o : %.s
	gcc -c -m32 -ffreestanding -o $@ $<
	
%.64.o : %.s
	gcc -c -m64 -ffreestanding -o $@ $<
	
clean:
	rm -f *~
	rm -f *.o
	rm -f */*/*.o
	rm -f *.elf
	rm -f *.bin

	
	
