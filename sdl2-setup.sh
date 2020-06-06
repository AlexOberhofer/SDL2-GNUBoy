#!/bin/bash

echo "Downloading SDL2-mingw32 cross-compiler libraries..."

mkdir ./lib/sdl2-mingw64/

cd ./lib/sdl2-mingw64/

wget https://www.libsdl.org/release/SDL2-devel-2.0.12-mingw.tar.gz

tar -xvzf SDL2-devel-2.0.12-mingw.tar.gz

rm SDL2-devel-2.0.12-mingw.tar.gz