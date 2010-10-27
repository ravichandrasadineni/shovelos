
LONG_OBJS = go_real.long.o
 
REAL_OBJS = entry.real.o \
		main.real.o \
		print.real.o \
		mmap.real.o \
		a20.real.o \
		alloc.real.o \
		mem.real.o \
		math64.real.o \
		bios_disk.real.o \
		pt.real.o \
		gdt.real.o \
		go_long.real.o 

.PHONY: all clean

all : stage1.5.bin

# Convert an elf to a flat
%.bin : %.elf
	objcopy -O binary $< $@

# Link real and long mode objects into an elf.
stage1.5.elf : $(OBJS) link.script
	$(LD) -Map link.map -melf_i386 -static -nodefaultlibs -nostdlib --nmagic -Tlink.script -o stage1.5.elf $(REAL_OBJS) $(LONG_OBJS)
	
# Compile a real-mode source file.
%.real.o : %.c
	gcc -c -std=c99 -m32 -Os -fomit-frame-pointer -fno-unroll-loops -ffreestanding -nostdinc -o $@ $<

# Compile a long-mode source file.
%.long.o : %.c
	gcc -c -std=c99 -m64 -Os -fomit-frame-pointer -fno-unroll-loops -ffreestanding -nostdinc -o $@ $<

# Assemble a real-mode object.
%.real.o : %.s
	gcc -c -m32 -ffreestanding -o $@ $<

# Assemble a long-mode object.
%.long.o : %.s
	gcc -c -m64 -ffreestanding -o $@ $<

clean:
	rm -f *~
	rm -f *.o
	rm -f stage1.5.elf
	rm -f stage1.5.bin