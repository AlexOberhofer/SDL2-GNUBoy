# Script to generate AppImage for sdl2gnuboy

export ARCH=$(uname -m)

if [ ! -d ./build ]; then mkdir ./build; fi

cd build

cp ../sys/linux/AppRun ./

chmod a+x AppRun

cp ../sys/linux/gameboy.png ./

cp ../sys/linux/sdl2gnuboy.desktop ./

cd ../

if [ ! -x ./build/appimagetool-$ARCH.AppImage ]; then
  curl -L -o ./build/appimagetool-$ARCH.AppImage https://github.com/AppImage/AppImageKit/releases/download/continuous/appimagetool-$ARCH.AppImage
  chmod a+x ./build/appimagetool-$ARCH.AppImage 
fi

cd build

./appimagetool-$ARCH.AppImage $PWD



