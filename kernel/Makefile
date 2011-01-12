
.PHONY: all clean arch/arch.a boot/boot.a mm/mm.a
all : shovelos.kernel

SRC64_C = main.c
LIBS    = arch/arch.a boot/boot.a mm/mm.a

export KERNEL_ROOT := $(shell pwd)
export CFLAGS := -I$(KERNEL_ROOT) -Wall -Werror -std=c99 -m64 -Os -mcmodel=kernel -mno-red-zone -mno-mmx -mno-sse -mno-sse2 -mno-sse3 -mno-3dnow -ffreestanding -nostdinc -nostdlib

OBJ64=$(SRC64_S:.s=.s.64.o) $(SRC64_C:.c=.c.64.o)

# Convert an elf to a flat
%.kernel : %.elf
	@echo Flatening $@
	@objcopy -O binary $< $@

shovelos.elf : $(OBJ64) $(LIBS) link.script
	@echo Linking   $@
	@$(LD) -s -Map link.map -static -nodefaultlibs -nostdlib --nmagic -Tlink.script -o $@ $(OBJ64) $(LIBS)

	 
	
%.c.64.o : %.c
	@echo Compiling $<
	@gcc $(CFLAGS) -c -o $@ $<
	
%.64.o : %.s
	@echo Assembling $<
	@gcc -Wall -c -m64 -ffreestanding -o $@ $<
	
arch/arch.a:
	@make -C arch arch.a
	
boot/boot.a:
	@make -C boot boot.a
	
mm/mm.a:
	@make -C mm mm.a
	
clean:
	@rm -f *~
	@rm -f *.o
	@rm -f *.elf
	@rm -f *.bin
	@make -C arch clean
	@make -C mm clean
	@make -C boot clean

	
	
	