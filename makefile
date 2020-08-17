.PHONY: all
.PHONY: clean

.PHONY: windows
windows:
	make -f makefile.windows
	rm -f sys/*/*.o src/*.o

.PHONY: linux
linux:
	make -f makefile.linux
	rm -f sys/*/*.o src/*.o

.PHONY: osx
osx:
	make -f makefile.osx
	rm -f sys/*/*.o src/*.o

clean:
	rm -f *gnuboy sdl2gnuboy.exe mac-sdl2gnuboy gmon.out *.o sys/*.o sys/*/*.o src/*.o