
 
OBJS = entry.o main.o print.o mmap.o a20.o alloc.o mem.o math64.o bios_disk.o pt.o gdt.o go_long.o 
#go_real.o

.PHONY: all clean

all : stage1.5.bin

%.bin : %.elf
	objcopy -O binary $< $@

stage1.5.elf : $(OBJS) link.script
	$(LD) -Map link.map -melf_i386 -static -nodefaultlibs -nostdlib --nmagic -Tlink.script -o stage1.5.elf.tmp $(OBJS)
	objcopy -O elf64-x86-64 stage1.5.elf.tmp
	gcc -c -ffreestanding -nostdinc -o go_real.o go_real.s
	$(LD) -Map link.map -melf_x86_64 -static -nodefaultlibs -nostdlib --nmagic -Tlink.script -o stage1.5.elf stage1.5.elf.tmp
	# go_real.o
#	$(LD) -Map link.map -melf_i386 -static -nodefaultlibs -nostdlib --nmagic -Tlink.script -o stage1.5.elf stage1.5.elf.tmp
#	$(LD) -Map link.map -melf_x86_64 -static -nodefaultlibs -nostdlib --nmagic -Tlink.script -o stage1.5.elf $(OBJS)
	
clean:
	rm -f *~
	rm -f *.o
	rm -f stage1.5.elf
	rm -f stage1.5.bin

%.o : %.c
	gcc -c -std=c99 -m32 -Os -fomit-frame-pointer -fno-unroll-loops -ffreestanding -nostdinc -o $@ $<
#	objcopy -O elf64-x86-64 $@


%.o : %.s
	gcc -c -std=c99 -m32 -Os -fomit-frame-pointer -fno-unroll-loops -ffreestanding -nostdinc -o $@ $<
#	objcopy -O elf64-x86-64 $@

