#########
# Package Script for Github build processes
# (C) 2020 Alex Oberhofer
# GPLv2
#########


#!/bin/bash

echo "Package Windows"

mkdir ./windows

mv ./sdl2gnuboy.exe ./windows

cp ./README.md ./windows

cd ./windows

wget https://www.libsdl.org/release/SDL2-2.0.14-win32-x64.zip

unzip SDL2-2.0.14-win32-x64.zip 

rm SDL2-2.0.14-win32-x64.zip 

zip  windows.zip *.*

mv windows.zip ../

cd ..

rm -rf windows
