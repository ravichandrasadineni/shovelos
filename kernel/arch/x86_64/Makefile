.PHONY: clean
.SECONDARY:

LIBS    = lapic/lapic.a acpi/acpi.a
SRC64_C = gdt.c idt.c console.c 8259.c pt.c mp.c ioapic.c ticket_lock.c isr.c kbc.c kbc_scancodes.c hpet.c
SRC64_S = gdt.s 

OBJ64=$(SRC64_S:.s=.s.64.o) $(SRC64_C:.c=.c.64.o)

%.a : $(OBJ64) $(LIBS)
	@echo Archiving $@
	@ar csrT $@ $(OBJ64) $(LIBS) 
	
%.c.64.o : %.c
	@echo Compiling $<
	@gcc $(CFLAGS) -c -o $@ $<
	
%.s.64.o : %.s
	@echo Assembling $<
	@gcc -Wall -c -m64  -mcmodel=kernel -o $@ $<
	
lapic/lapic.a:
	@make -C lapic lapic.a
	
acpi/acpi.a:
	@make -C acpi acpi.a
	
clean:
	@rm -f *~
	@rm -f *.o
	@rm -f *.a
	@make -C lapic clean
	@make -C acpi clean

	