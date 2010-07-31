
CFLAGS = -m32 -Os -fomit-frame-pointer -fno-unroll-loops -ffreestanding -nostdinc 

OBJS = entry.o main.o print.o mmap.o

.PHONY: all clean

all : stage1.5.bin

%.bin : %.elf
	objcopy -O binary $< $@

stage1.5.elf : $(OBJS) link.script
	$(LD) -melf_i386 -static -nodefaultlibs -nostdlib --nmagic -Tlink.script -o stage1.5.elf $(OBJS)
	
clean:
	rm -f *~
	rm -f *.o
	rm -f stage1.5

%.o : %.c
	gcc -c $(CFLAGS) -o $@ $<

%.o : %.s
	gcc -c $(CFLAGS) -o $@ $<

