bsector.bin : bsector.elf
	objcopy -O binary bsector.elf bsector.bin

bsector.o : bsector.s
	as bsector.s -o bsector.o

bsector.elf : bsector.o
	ld -static -Tlink.script -Map link.map -nostdlib --nmagic -o bsector.elf bsector.o

clean :
	rm -f bsector.o
	rm -f bsector.elf
	rm -f bsector.bin
	rm -f *~

