install_mbr : mbr/mbr.bin
	dd if=mbr/mbr.bin of=ATA0.img bs=446 count=1 conv=notrunc

mbr/mbr.bin : mbr/mbr.s mbr/Makefile mbr/link.script
	make -C mbr mbr.bin

