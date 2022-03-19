# GNUBoy SDL2 Port

# Overview 

![GNU/Linux](https://github.com/AlexOberhofer/SDL2-GNUBoy/workflows/GNU/Linux/badge.svg)<br>
![OSX](https://github.com/AlexOberhofer/SDL2-GNUBoy/workflows/OSX/badge.svg)<br>
![Windows MinGW-w64](https://github.com/AlexOberhofer/SDL2-GNUBoy/workflows/Windows%20MinGW-w64/badge.svg)<br>


A Multiplatform Gameboy Emulator. Free Software.

GNUBoy is a cool, old piece of software. This fork focuses on a multiplatform SDL 2 port. Hopefully this codebase can be helpful to someone in the future
or someone interested in learning about C, SDL, or emulator development.

Fork or contribute! Based on GNUBoy and licensed under GNU GPLv2

<div align="center">
    <img alt="Blue" src="https://github.com/AlexOberhofer/SDL2-GNUBoy/raw/master/docs/img/blue.PNG">
</div>

# Releases

The latest packaged releases are available under the associated workflow. This currently is only for Windows. If you use Linux/MacOS BUILD IT!!!!

Download Versioned Releases: (https://github.com/AlexOberhofer/SDL2-GNUBoy/releases)

Builds: (https://github.com/AlexOberhofer/SDL2-GNUBoy/actions) <br>
*Note* Builds only provided for windows via mingw64 for each commit <br>
*Note* The stability of these builds may vary <br>

SDL2 GNUboy pre-compiled binaries will be provided with each release. The source is also provided on the release page and the binaries can be built from the source or from this repository.

For bleeding edge SDL2-GNUBoy - build the master branch. Some stable versions also have branches to build from if need be. Otherwise check the release tab for Windows binaries or build it "the unix way" for *nix / BSD / OSX.

v1.2.2
- GameBoy and GameBoy Color Bios
- SDL video bugfixes
- Restored fullscreen toggle
- Link cable emulation (*nix)

v1.2.1
- Audio Bugfixes (No more popping noise on startup)
- Audio Interface updates
- Optional confirmation window when closing emulator session

v1.2.0

Thanks to [Ryzee119](https://github.com/Ryzee119) for his contributions! 

- Many SDL bug fixes
- Fullscreen / RC file fixes
- Key rebinding now works
- Integer scaling (new integer_scale rc variable)
- Joystick/Gamepad rewrite (new joy_deadzone rc variable)
- Rumble Support (new joy_rumble_strength rc variable)

v1.1.1
- Joystick/Gamepad Support 
- New GB classic pallette (Can be changed with rc file)
- Renderer + Keyboard bugfixes

v1.1.0
 - Builds for more platforms
 - CPU logic fixes
 - Packaging + install scripts
 - SDL2 hardware rendering + scaling

v1.0
 - Supports Linux only

# Screenshots

<div align="center">

|  GB | GBC  |
|---|---|
| <img alt="Dr Mario" src="https://github.com/AlexOberhofer/SDL2-GNUBoy/raw/master/docs/img/drmario.PNG"> | <img alt="Lemmings" src="https://github.com/AlexOberhofer/SDL2-GNUBoy/raw/master/docs/img/lemmings.PNG"> |
|<img alt="Super Mario :amd" src="https://github.com/AlexOberhofer/SDL2-GNUBoy/raw/master/docs/img/supermarioland.PNG">|<img alt="Pokemon Yellow" src="https://github.com/AlexOberhofer/SDL2-GNUBoy/raw/master/docs/img/pkmnyellow.PNG">| 
|   <img alt="Tetris" src="https://github.com/AlexOberhofer/SDL2-GNUBoy/raw/master/docs/img/tetris.PNG">|<img alt="Pokemom Crystal" src="https://github.com/AlexOberhofer/SDL2-GNUBoy/raw/master/docs/img/pkmncrystal.PNG">|

</div>

# Performance

The emulation core of GNUBoy is mainly unmodified except for a number of bugfixes I have ported into this fork. 

## Accuracy

A number of cpu buxfixes were ported from [here](https://github.com/mattkj/super-go-play/tree/master/gnuboy-go/components/gnuboy)

The emulator currently fails only one CPU instruction via blarggs test rom (Windows executable running in wine on Linux):
<div align="center">
    <img src="https://github.com/AlexOberhofer/SDL2-GNUBoy/raw/master/docs/img/blargg.PNG">
</div>

## SDL2GNUBoy v1.2.1

Improvements for v1.2.1: 

- Now compiles for Apple M1 chip.
- SDL2 audio bugfixes
- Better controller support + rumble (v1.2.0)

<div align="center">
    Pokemon Pinball (Now with rumble) <br>
    <img alt="Pinball" src="https://github.com/AlexOberhofer/SDL2-GNUBoy/raw/master/docs/img/pinball.gif">
</div>


SDL2GNUBoy has been compiled and tested on the following platforms:

- Windows (x86/ARM)
- Mac (x86/ARM)
- Linux (x86/ARM)
- WSL 

# Status

Version 1.2.2 is in development. See the master branch for bleeding edge SDL2-GNUBoy.

## Whats been done

(At this point - this is only a subset of what actually has been done here... but below is a sample)

- Multiplatform builds

- SDL2 input implementation

- SDL1.2 rendering ported to SDL2

- SDL1.2 sound ported to SDL2 


## Todo

- Porting: Emscripten?

- Add handler for zip files (non-zlib)

# Configuring

Sample configuration files are provided in the /rc/ directory of this repository.

Setup configuration from sample (From repository root)

```
$ cp ./rc/gnuboy.rc.sample ./gnuboy.rc
```

Example commands and color pallettes are also available in the /rc/ directory.

# Configuring Boot ROM

The following bootrom files have been tested, but others may work.

|  Platform | Name | SHA1  | Notes |
|---|---|---|---|
| GB | dmg_boot.bin | 4ED31EC6B0B175BB109C0EB5FD3D193DA823339F | |
| GB | mgb_boot.bin | 4E68F9DA03C310E84C523654B9026E51F26CE7F0 | Gameboy Pocket  |
| GBC | cgb_boot.bin | 1293D68BF9643BC4F36954C1E80E38F39864528D | |
 
# Building

Build scripts are provided for both native unix applications and for cross compiling via mingw-w64 gcc

Build instructions are provided for both below:

## Mac

See the "Linux section" except for a few differences. The Mac build uses Clang by default. Build has been tested on both Intel and M1 Macintoshes. 

Installing SDL2:

```
$ brew install sdl2
```

Building Mac Binar:y

```
$ make osx
```
 

## Linux 

This project requires GCC and SDL2 to build for linux. A Makefile has been provided.

Install SDL2 development libraries (Ubuntu):

```
$ apt-get install libsdl2-dev
```

Clone github repository: 

```
$ git clone https://github.com/AlexOberhofer/SDL2-GNUBoy.git
```

Enter project directory:

```
$ cd sdl2-gnuboy
```

Build:

```
$ make linux
```

Run:

```
$ ./sdl2gnuboy ./rom
```

## Windows (Cross Compiler)

SDL2 GNUboy can be cross compiled for Windows on Linux (or the Windows Linux Subsystem)

This requires mingw64-w64-gcc and the mingw SDL2 libraries. 

These can be installed via the AUR or the SDL website (Extract and place contents in /usr/x86_64-w64-mingw32)


Install libraries (from AUR using yay)

```
$ yay mingw-w64-gcc
```

Install MinGW-w64-sdl2

```
$ yay mingw-w64-sdl2
```

Clone github repository: 

```
$ git clone https://github.com/AlexOberhofer/SDL2-GNUBoy.git
```

Enter project directory:

```
$ cd sdl2-gnuboy
```

Build:

```
$ make windows
```

Run  

Windows: 

  - Drag and drop rom over executable

(Wine):

```
$ wine ./sdl2gnuboy.exe ./rom
```

Run (cmd):

```
$ ./sdl2gnuboy.exe ./rom
```

# Controls (Keyboard)

Controls are as follows:

```
ESC - QUIT 
```

```
W - UP
```

```
S - DOWN
```

```
A - LEFT 
```

```
D - RIGHT 
```

```
Q - A
```

```
E - B
```

```
ENTER - START
```

```
X - SELECT
```

# Controls (Gamepad)

Controls are as follows (Based on Xbox 360 controller layout):

```
Joy/Pad Up - UP
```

```
Joy/Pad Down - DOWN
```

```
Joy/Pad Left - LEFT 
```

```
Joy/Pad Right - RIGHT 
```

```
B - A
```

```
A - B
```

```
Start - START
```

```
Back - SELECT
```

## Porting

The GNUBoy core is extremely fast and portable. Porting documentation can be found in the docs folder.

### Repository Directory
  - /lib - Library Code
  - /sys - System dependent code
  - /src - Core emulator logic
  - /include - GNUBoy Header files
  - /docs - Various docs + media from GNUBoy

## Developers

Alex Oberhofer

## Acknowledgements

See CREDITS for full list of contributors.

Thank you to the original GNUBoy developers. Please see the history branch for their versions. Legacy documentation can be found in the docs folder.