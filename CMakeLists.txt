cmake_minimum_required(VERSION 3.13)
project(sdl2gnuboy)

if(MSVC OR MSYS OR MINGW)
        set(TARGET_NAME sdl2gnuboy.exe)
else()
        set(TARGET_NAME sdl2gnuboy)
endif()


set(CMAKE_C_FLAGS "-O3 -Wall -std=c99")

find_package(SDL2 REQUIRED)

include_directories(./include)
include_directories(./lib)
include_directories(./)
include_directories("/usr/local/include/")

include_directories(${SDL2_INCLUDE_DIR})

add_definitions(-DIS_LITTLE_ENDIAN  -DIS_LINUX -D_GNU_SOURCE=1 -D_REENTRANT)

add_executable(${TARGET_NAME} sys/sdl2/sdl-video.c sys/sdl2/sdl-audio.c sys/sdl2/sdl-input.c ./src/lcd.c ./src/refresh.c
        ./src/lcdc.c ./src/palette.c ./src/cpu.c ./src/mem.c ./src/rtc.c ./src/hw.c ./src/sound.c
        ./src/events.c ./src/keytable.c ./src/loader.c ./src/save.c ./src/debug.c
        ./src/emu.c ./src/main.c ./src/rccmds.c ./src/rckeys.c ./src/rcvars.c ./src/rcfile.c ./src/exports.c
        ./src/split.c ./src/path.c ./lib/gz/inflate.c lib/xz/xz_crc32.c lib/xz/xz_crc64.c lib/xz/xz_dec_lzma2.c
        lib/xz/xz_dec_stream.c lib/xz/xz_dec_bcj.c sys/nix/nix.c)

target_link_libraries(${TARGET_NAME} ${SDL2_LIBRARIES})