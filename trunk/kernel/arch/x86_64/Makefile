.PHONY: clean

SRC64_C = gdt.c idt.c kprintf.c 8259.c
SRC64_S = gdt.s idt.s isr.s

OBJ64=$(SRC64_S:.s=.s.64.o) $(SRC64_C:.c=.c.64.o)

%.a : $(OBJ64)
	@echo Archiving $@
	@ar rcs $@ $(OBJ64)
	
%.c.64.o : %.c
	@echo Compiling $<
	@gcc -I$(KERNEL_ROOT) -Wall -c -std=c99 -m64 -Os -mcmodel=large -mno-red-zone -mno-mmx -mno-sse -mno-sse2 -mno-sse3 -mno-3dnow -ffreestanding -nostdinc -nostdlib -o $@ $<
	
%.s.64.o : %.s
	@echo Assembling $<
	@gcc -Wall -c -m64  -mcmodel=large -o $@ $<
	
clean:
	@rm -f *~
	@rm -f *.o
	@rm -f *.a

	