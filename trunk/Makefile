install_bootloader : bootsector/bsector.bin
	dd if=bootsector/bsector.bin of=ATA0.img bs=446 count=1 conv=notrunc

bootsector/bsector.bin : bootsector/bsector.s
	make -C bootsector bsector.bin

