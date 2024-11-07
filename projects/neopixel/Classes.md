# Class Reference 
To use NeoPixel strip, LEDs of M5AtomMatrix and NECO unit.

Note that **mruby/c does not distingush between class methods and instance methods.**

## Implemented classes

### NeoPixel (c_neopixel.cpp)
25 NeoPixels of M5AtomMatarix is connected to GPIO27.

Call show() to apply pixel colors on NeoPixel LEDs.

- new(led_num, gpio_num=_PortA_Pin2_)
  > Creates a new instance to control neopixel leds connected to `gpio_num`.
  > So NeoPixel.new(25,27) is for M5AtomMatrix.
  > If you connect NECO unit to PortA, NeoMatrix.new(70) is used. (two Nekomimis)
- set_pixel_color(n, r, g, b)
  > Sets color(`r`,`g`,`b`) on led positioned at `n`.
- map1(ary)
  > Sets LED colors by `ary` where the format is \[ \[r,g,b\] [r,g,b\],\[r,g,b\]... \]
- map3(ary_r, ary_g, ary_b)
  > Sets LED colors by `ary` for each color where the format is \[ c1,c2,c3,... \]

- show
  > Applies the colors on LEDs.
- fill(r,g,b,st,en)
  > Fills the colors on LEDs positioned from `st` to `en`
- clear
  > Clears the colors.

