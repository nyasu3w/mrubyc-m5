# mruby/c for m5
This is a project for using mruby/c on M5Stack devices with M5Unified library on Arduino framework. Forked from the original mruby/c and added classes for the purpose.

Also targeting on easy implementation with Arduino libraries.

The below movie is a(n old) total_demo in this project on M5Stack Core2:

https://github.com/user-attachments/assets/b0aa8a82-fc44-421c-a1d7-70b15c6e211f

## prerequisites
- mruby
- platformio

mruby needs to byte compile ruby files.
platformio is necessary because the build system depends on it.

## Features 
Supporting ruby scripts (mruby/c) for:
- Writing chars/Drawing graphs on LCD 
- Support of BtnA/B/C
- Support of Touch panel
- Read/Write SD cards (and SPIFFS)
- Reading IMU value
- Easy extention with many arduino libraries

The compactness is important and implemented functions are rather little, but you can implement/extend more for your purpose. Though, this project do not pursue compactness so far. Many heaps are used for arduino libraries other than mruby/c memory pool.

## Example projects
Placed in "projects" directory.
- demo of functions (total_demo)
- sample project for trying function limitation (only_display)
- adding other fonts to system (newfont)
- extension for M5Stack CardKB (cardkb)
- loading compiled rubyscript (mrb) from SD (mrb_in_sd)
- extension of M5Stack-Avatar (m5stack-avatar)
- extension for Encoder of M5Dial (m5dial)
- extension for wifi to http get (wifi)
- extension for accessing SPIFFS (spiffs)
- extension for neopixel strip
- EspNow, Speaker (sample_rb)

## directory structure
    mrubyc-m5/
       +- lib/
       |   +- mrubyc-m5 : main library of this project
       +- projects/     : example projects of mruby/c-m5
       |   + total_demo
       |   + spiffs
       |   + carddb
       |   + m5dial
       |   + m5stack-avatar
       |        :
       |
       +- src : almost same as original mruby/c. hal.c and hal.h are added
       | 
       the other directories are the same as original mruby/c

## For the first try
Build by PlatformIO in the top directory, then upload to your M5Stack device.

If you have already installed mruby and PlatformIO in the PATH, you are ready. 

`platformio.ini` is already set to build `project/total_demo` sample.
You can change the built sample by editing `src_dir` variable in `platformio.ini`

When src_dir is changing, VSCode might need some seconds to apply the change. If VSCode notifies such like "unknown ids", try again a few seconds later.

Please note that the directory structure is not as usual. It might be my mistake but it works for me for the moment.

Each project files are placed under projects directory. Now some projects are there, and one of them is selected in src_dir variable in `platformio.ini` in the top directory.

### preparation
At first, please install the prerequisites above.

`mrbc` command from mruby is necessary in your PATH. `extra_script.py` in the build system calls it to byte-compile ruby file.

The code in the script is:
> ret=subprocess.run([ 'mrbc', ’-B', 'bname', 'rubyfile']) 

If this line got "file not found" error or such error, please adjust the parameters for your environment.

`extra_script.py` will check the necessity and do byte compile.
If you do not like the automatic byte compile, you can do it by hand:

> mrbc -B {name} {name}.rb

`{name}.c` is generated. The generated C file is to be on src directory of the project. {name} will be specified in your cpp file.

In that case, please remove a line to call before_build function in near the bottom of `extra_script.py`. Otherwise, the build system would stop.


### build
If you try this with VisualStudio Code, just do "PlatformIO: Build" command or hit check mark on the bottom status line of vscode.

Confirm the board setting in `platformio.ini`. You might want to change default_envs variable. 

If you using CLI of Platform IO, the board can be selected by the command line option `-e <envname>` for pio command.

> pio run -e `your-board-name-in-platformio.ini`

Simple. Thank you, PlatformIO.

Perhaps `pio` command name is an alias of `platformio.exe` on Windows.

In the case of manual mrbc activation above, run the mrbc command before `pio run`.

You can upload the built firmware by

> pio run --target=upload -e `your-board-name-in-platformio.ini`

If you are using PlatformIO on VSCode,you can build and upload from the bottom line of VSCode.

## Creating a new project
1. make a new directory in "projects"
1. If you need additional libraries, write it in 'libdeps.txt' in the new directory
  1. `libdeps.txt` format is same as lib_deps variable in `platformio.ini`
1. create your source files in src directory in the new directory.

### Configuration of vm
See a main.cpp from example projects under `projects` directory.
Those are almost same and 

1. MEMSIZE can change the size of memory pool for vm. Note that this is only for mruby/c itself.
1. Activated functions(SD,EspNow,SPIFSS and so on) are defined in `lib/mrubyc-m5/libconfig.h`. See the file. Some functions are disabled as default.
1. New implementation of classes of arduino libraries is acceptable like m5stack-avatar and m5dial project.
1. if *.rb file is in the `src` directory, those are byte compiled to mrb files and the code is accessed as a array of the same name.  For convinience, `mrubycode` is the coded array name which is converted from such as `#define mrubycode demo`, where demo is from demo.rb.

### Configuration List
These are defined in `libconfig.h`. Comment or uncomment to configure.
Additional CJK fonts and USE_ESPNOW may costs too much flash memory.

| macro | default | purpose |
|-------|---------|---------| 
|USE_FULL_BUTTONS|off| to enable BtnEXT and BtnPWR|
|USE_DISPLAY_GRAPHICS|on| to support more graphics functions |
|USE_FONT |on| to support Font functions |
|USE_EFONTJA10|off| to prepare Japanese efontJA10 |
|USE_IMU_FUNCTION|on| to support M5Unified IMU functions|
|USE_SD_FUNCTION|on| to support SD functions (need File functions)|
|USE_FILE_CLASS|on| to support File functions|
|USE_TOUCH|on|to support Touch panel functions|
|USE_SPEAKER|on|to support Speaker functions|
|USE_CANVAS|on|to support Canvas(Sprite) function
|USE_MULTIDISPLAY|on| to support multiple displays|
|USE_ESPNOW|off| to support ESP Now |
|USE_TEMPORAL_RANDOM_FUNCTION | on | to support random number function | 


## Reference
Class reference implemented for M5Unified and Arduino libraries.

[Classes.md](lib/mrubyc-m5/Classes.md)

*** 
### The original mruby/c README follows.

---
# mruby/c

[![Ruby](https://github.com/mrubyc/mrubyc/actions/workflows/test.yml/badge.svg)](https://github.com/mrubyc/mrubyc/actions/workflows/test.yml)

[![Join the chat at https://gitter.im/mrubyc/mrubyc](https://badges.gitter.im/mrubyc/mrubyc.svg)](https://gitter.im/mrubyc/mrubyc?utm_source=badge&utm_medium=badge&utm_campaign=pr-badge&utm_content=badge)

mruby/c is another implementation of mruby.

- Small memory consumption
- Limited class libraries
- Small size rather than execution speed

### Comparison between mruby and mruby/c

||mruby/c|mruby|
|----|----|----|
|memory size| < 40KB | < 400KB |
|main target| one-chip microprocessors | general embedded software|


## Documents

[How to compile?](doc/compile.md)

[How to run tests?](doc/test.md)


## Developer team

- [Shimane IT Open-Innovation Center](http://www.s-itoc.jp/)
- [Kyushu Institute of Technology](http://www.kyutech.ac.jp/)

## License

mruby/c is released under the Revised BSD License(aka 3-clause license).

## Related work

- Device classes for mruby/c (https://github.com/mrubyc/dev)
- Some sample programs that mainly control sensors. (https://github.com/mrubyc/devkit02/tree/main/samples)
