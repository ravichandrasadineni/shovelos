
.PHONY: all clean arch/arch.a
all : shovelos.kernel

SRC64_C = main.c
LIBS    = arch/arch.a

export KERNEL_ROOT := $(shell pwd)

OBJ64=$(SRC64_S:.s=.64.o) $(SRC64_C:.c=.64.o)

# Convert an elf to a flat
%.kernel : %.elf
	objcopy -O binary $< $@

shovelos.elf : $(OBJ64) $(LIBS) link.script
	$(LD) -s -Map link.map -static -nodefaultlibs -nostdlib --nmagic -Tlink.script -o $@ $(OBJ64) $(LIBS) 
	
%.64.o : %.c
	gcc -I$(KERNEL_ROOT) -Wall -c -std=c99 -m64 -Os -mcmodel=large -mno-red-zone -mno-mmx -mno-sse -mno-sse2 -mno-sse3 -mno-3dnow -ffreestanding -nostdinc -nostdlib -o $@ $<
	
%.64.o : %.s
	gcc -Wall -c -m64 -ffreestanding -o $@ $<
	
arch/arch.a:
	make -C arch arch.a
	
clean:
	rm -f *~
	rm -f *.o
	rm -f *.elf
	rm -f *.bin
	make -C arch clean

	
	
	