.PHONY: all install_vbr install_mbr clean

all: install_vbr install_mbr

install_vbr: vbr/vbr.bin
	dd if=vbr/vbr.bin of=ATA0.img seek=63 bs=512 count=1 conv=notrunc

install_mbr: mbr/mbr.bin
	dd if=mbr/mbr.bin of=ATA0.img bs=446 count=1 conv=notrunc

vbr/vbr.bin: vbr/vbr.s vbr/Makefile vbr/link.script
	make -C vbr vbr.bin

mbr/mbr.bin: mbr/mbr.s mbr/Makefile mbr/link.script
	make -C mbr mbr.bin

clean:
	make -C mbr clean
	make -C vbr clean

