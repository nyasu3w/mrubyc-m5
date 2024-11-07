# m5stack-avatar
This is a sample code to use another arduino library with mruby/c.

## point to see
- `c_avatar.cpp` and `c_avatar.h` are the additional code to use m5avatar library
- "M5Stack-Avatar" is written in `libdeps.txt` to import the library by platformio
- `main.cpp` to include `c_avatar.h`
- `main.cpp` to use class_avatar_init() after my_mrubyc_init()
  * class_avatar_init() is declared in `c_avatar.h` and defined in `c_avatar.cpp`
- it is ok if you put `c_avatar.cpp` and `c_avatar.h` in lib/mrubyc-m5/src and modify `main.cpp` as above

## class reference
[Classes.md](Classes.md)