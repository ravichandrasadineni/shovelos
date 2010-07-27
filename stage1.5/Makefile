
CFLAGS = -Os -fomit-frame-pointer -fno-unroll-loops -ffreestanding -nostdinc -save-temps

OBJS = entry.o main.o print.o

.PHONY: all clean

all : stage1.5.bin

%.bin : %.elf
	objcopy -O binary $< $@

stage1.5.elf : $(OBJS) link.script
	$(LD) -static -nodefaultlibs -nostdlib -Tlink.script -o stage1.5.elf $(OBJS)
	
clean:
	rm -f *~
	rm -f *.o
	rm -f stage1.5

%.o : %.c
	gcc -c $(CFLAGS) -o $@ $<

%.o : %.s
	gcc -c $(CFLAGS) -o $@ $<

