@echo off
make
3dstool -cvtf romfs romfs.bin --romfs-dir romfs
makerom -f cci -o 3DS_eBook_Reader.3ds -rsf gw_workaround.rsf -target d -exefslogo -elf 3DS_eBook_Reader.elf -icon icon.bin -banner banner.bin -romfs romfs.bin
makerom -f cia -o 3DS_eBook_Reader.cia -elf 3DS_eBook_Reader.elf -rsf cia_workaround.rsf -icon icon.bin -banner banner.bin -exefslogo -target t -romfs romfs.bin
pause