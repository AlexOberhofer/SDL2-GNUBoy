.PHONY: all
.PHONY: clean

.PHONY: linux
linux:
	make -f makefile.linux
	rm -f sys/*/*.o src/*.o lib/*/*.o

.PHONY: windows
windows:
	make -f makefile.windows
	rm -f sys/*/*.o src/*.o lib/*/*.o

.PHONY: mingw
mingw:
	make -f makefile.mingw
	rm -f sys/*/*.o src/*.o lib/*/*.o

.PHONY: osx
osx:
	make -f makefile.osx
	rm -f sys/*/*.o src/*.o lib/*/*.o

clean:
	rm -f *gnuboy sdl2gnuboy.exe mac-sdl2gnuboy gmon.out *.o sys/*.o lib/*/*.o sys/*/*.o src/*.o ./build
	mkdir build
