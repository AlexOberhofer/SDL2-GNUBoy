# GNUBoy SDL2 Port

# Overview 

![GNU/Linux](https://github.com/AlexOberhofer/SDL2-GNUBoy/workflows/GNU/Linux/badge.svg)
![Windows MinGW-w64](https://github.com/AlexOberhofer/SDL2-GNUBoy/workflows/Windows%20MinGW-w64/badge.svg)

A Multiplatform Gameboy Emulator. Free Software.

My personal fork for experimentation. Based on GNUBoy and licensed under GNU GPLv2

SDL2 port now working on Linux and Windows

The scripts in the root directory are used by github actions workflow for building this repository. Read the code before you execute it...

# Releases

The latest packaged releases are available under the associated workflow. This currently is only for Windows. If you use Linux/MacOS BUILD IT!!!!

Download Versioned Releases: (https://github.com/AlexOberhofer/SDL2-GNUBoy/releases)

Builds: (https://github.com/AlexOberhofer/SDL2-GNUBoy/actions)
*Note* Builds only provided for windows via mingw64 for each commit

SDL2 GNUboy pre-compiled binaries will be provided with each release. The source is also provided on the release page and the binaries can be built from the source or from this repository.

v1.1.0 (Coming soon)
 - Added Windows cross compilation

v1.0
 - Supports Linux only

# Screenshots

<div align="center">

|  GB | GBC  |
|---|---|
| <img alt="Dr Mario" src="https://github.com/AlexOberhofer/SDL2-GNUBoy/raw/master/docs/drmario.PNG"> | <img alt="Lemmings" src="https://github.com/AlexOberhofer/SDL2-GNUBoy/raw/master/docs/lemmings.PNG"> |
|<img alt="Ms Pac Man" src="https://github.com/AlexOberhofer/SDL2-GNUBoy/raw/master/docs/mspacman.PNG">|<img alt="Pacman" src="https://github.com/AlexOberhofer/SDL2-GNUBoy/raw/master/docs/pacman.PNG">| 
|<img alt="Super Mario :amd" src="https://github.com/AlexOberhofer/SDL2-GNUBoy/raw/master/docs/supermarioland.PNG">|<img alt="Pokemon Yellow" src="https://github.com/AlexOberhofer/SDL2-GNUBoy/raw/master/docs/pkmnyellow.PNG">| 
|   <img alt="Tetris" src="https://github.com/AlexOberhofer/SDL2-GNUBoy/raw/master/docs/tetris.PNG">|<img alt="Pokemom Crystal" src="https://github.com/AlexOberhofer/SDL2-GNUBoy/raw/master/docs/pkmncrystal.PNG">|
|<img alt="Zelda" src="https://github.com/AlexOberhofer/SDL2-GNUBoy/raw/master/docs/zelda.PNG"> |<img alt="Super Mario Deluxe" src="https://github.com/AlexOberhofer/SDL2-GNUBoy/raw/master/docs/supermariodeluxe.PNG"> |

</div>

# Performance

The original core of GNUBoy is mainly unmodified except for a number of bugfixes I have ported into this fork. 

## Accuracy

A number of cpu buxfixes were ported from [here](https://github.com/mattkj/super-go-play/tree/master/gnuboy-go/components/gnuboy)

The emulator currently fails only one CPU instruction via blarggs test rom (Windows executable running in wine on Linux):

<img align="center" src="https://github.com/AlexOberhofer/SDL2-GNUBoy/raw/master/docs/blargg.PNG">

## SDL2GNUBoy v1.1.0 Metrics

Coming Soon (Windows)

## SDL2GNUBoy v1.0 Metrics

Here are a few demos running game intro loops and a unix process monitor (htop):

*Note:* Captures are from a Manjaro VM running on VMWare on Windows 10 <br>
<center> <h4>Classic Gameboy: Pokemon Red</h4><br>
<img alt="Zelda" src="https://github.com/AlexOberhofer/SDL2-GNUBoy/raw/master/docs/red.gif"></center><br>
<center> <h4>Gameboy Color: Pokemon Crystal</h4><br>
<img alt="Zelda" src="https://github.com/AlexOberhofer/SDL2-GNUBoy/raw/master/docs/crystal.gif"></center><br>



## Legacy Metrics

Pre - fork GNUBoy performance metrics:

*Note*: The ASM code has now been removed from this repository

```
  PERFORMANCE

Here are some performance estimates i've gathered (given in percent
cpu utilization, running at full 60 fps)...

                                 Optimized C        Assembly
  AMD K6-2/450                       12%               8%
  Pentium/75                     (too slow)           70%
  SGI O2                             25%            (no asm)
  SGI Indy                           70%            (no asm)
  Sun Ultra60                       3-20%           (no asm)
  IBM S/390                      about 0.3%         (no asm)

Note that these figures were computed before sound was implemented.
Until the sound code is further optimized, gnuboy may run somewhat
slower than these numbers suggest.
```

# Status

A working SDL2 build can be generated from the provided makefile. This is the build file I use for linux

## Whats been done

- Builds and Runs on Linux (Tested and developed on Manjaro)

- Cross compiles on Linux into a Windows executable

- SDL2 control implementation (currently not rebindable)

- SDL1.2 rendering ported to SDL2 (Now uses gpu if possible and supports scaling)

- SDL1.2 sound ported to SDL2 (Needs a bit of work but its functional)


## Todo

- Porting: The codebase is currently buildable on Windows/MacOS/*nix. I have built the SDL2 version of the codebase on each of those platforms, but I have not yet provided build scripts or releases for multiplatform. The SDL2 version only currently works on linux.

- Joystick / Controller Support

- SDL Sound rewrite

- SDL Keys rewrite (To support key rebinding)

- Fullscreen support

- GTK/QT? Backend

- Fix Sound MacOS


# Building

Build scripts are provided for both native unix applications and for cross compiling via mingw-w64 gcc

Build instructions are provided for both below:

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
$ cd gnuboy
```

Build:

```
$ make 
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
$ cd gnuboy
```

Build:

```
$ make -f makefile.windows
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

# Controls

Controls are as follows:

```
P - PAUSE GAME (currently unsupported)
```

```
R - RESUME GAME (currently unsupported)
```

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
BACKSPACE - SELECT
```


## Developers

### SDL2 GNUBoy

Alex Oberhofer

## Acknowledgements

Cinoop Project 

CTurt - [Github](https://github.com/CTurt)

* [Github](https://github.com/CTurt/Cinoop) 

* [Website](https://cturt.github.io/cinoop.html)

GNUBoy

Thank you to the original GNUBoy developers. Please see the history branch for their versions. Legacy documentation can be found in the docs folder.

* [Original Forked Repo](https://github.com/AlexOberhofer/sdl2gnuboy)

* [Website](https://sourceforge.net/projects/gnuboy/)

Thanks to rofl0r for xz compression, build fixes, code cleanup, and more. See his code below:

* [Updated GNUBoy Repository] (https://github.com/rofl0r/gnuboy)

