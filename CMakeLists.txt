cmake_minimum_required(VERSION 3.13)
project(sdl2gnuboy)

set(TARGET_NAME sdl2gnuboy)

#If Linux / UNIX / BSD / APPLE / etc...
if(UNIX OR APPLE) 
        set(PLATFORM_IMPL sys/nix/nix.c)
#If not unix probably windows?
else() 
        set(PLATFORM_IMPL sys/windows/windows.c)
        add_definitions(-DALT_PATH_SEP)
endif()

find_package(SDL2 REQUIRED)

set(CMAKE_C_STANDARD 99)

set(CMAKE_C_FLAGS "-Wall")

set(CMAKE_C_FLAGS_RELEASE "-O3")

set(CMAKE_C_FLAGS_DEBUG "-g3")

include_directories(./include)
include_directories(./lib)
include_directories(./)
include_directories("/usr/local/include/")

include_directories(${SDL2_INCLUDE_DIR})

add_definitions(-DIS_LITTLE_ENDIAN -D_GNU_SOURCE=1 -D_REENTRANT)

add_executable(${TARGET_NAME} sys/sdl2/sdl-video.c sys/sdl2/sdl-audio.c sys/sdl2/sdl-input.c ./src/lcd.c ./src/refresh.c
        ./src/lcdc.c ./src/palette.c ./src/cpu.c ./src/mem.c ./src/rtc.c ./src/hw.c ./src/sound.c
        ./src/events.c ./src/keytable.c ./src/loader.c ./src/save.c ./src/debug.c
        ./src/emu.c ./src/main.c ./src/rccmds.c ./src/rckeys.c ./src/rcvars.c ./src/rcfile.c ./src/exports.c
        ./src/split.c ./src/path.c ./lib/gz/inflate.c lib/xz/xz_crc32.c lib/xz/xz_crc64.c lib/xz/xz_dec_lzma2.c
        lib/xz/xz_dec_stream.c lib/xz/xz_dec_bcj.c ${PLATFORM_IMPL})

target_link_libraries(${TARGET_NAME} ${SDL2_LIBRARIES})