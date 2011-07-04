.PHONY: clean

SRC64_C = gdt.c idt.c console.c 8259.c pt.c acpi.c mp.c ioapic.c ticket_lock.c isr.c
SRC64_S = gdt.s idt.s

OBJ64=$(SRC64_S:.s=.s.64.o) $(SRC64_C:.c=.c.64.o)

%.a : $(OBJ64)
	@echo Archiving $@
	@ar rcs $@ $(OBJ64)
	
%.c.64.o : %.c
	@echo Compiling $<
	@gcc $(CFLAGS) -c -o $@ $<
	
%.s.64.o : %.s
	@echo Assembling $<
	@gcc -Wall -c -m64  -mcmodel=kernel -o $@ $<
	
clean:
	@rm -f *~
	@rm -f *.o
	@rm -f *.a

	