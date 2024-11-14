# Class Reference 

Note that **mruby/c does not distingush between class methods and instance methods.**

## Implemented classes

### NeoPixel (c_neopixel.cpp)
A class to use NeoPixel strip, LEDs of M5AtomMatrix and NECO unit.
When the object is not necessary, call `destroy` method to free its memory. The destroyed object is not valid.

Call show() to apply pixel colors on NeoPixel LEDs.

For ref, 25 NeoPixels of M5AtomMatarix is connected to GPIO27.

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
- destroy
  > Deletes the object to free memory. The destroyed object is no longer valid.
