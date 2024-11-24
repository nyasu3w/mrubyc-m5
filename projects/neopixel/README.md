# neopixel
A sample for neopixel strip.
This sample is for M5Stack NECO unit. (70 leds)

Almost all functions except USE_TEMPORAL_RANDOM_FUNCTION can be disabled in libconfig.h for this sample.

https://github.com/user-attachments/assets/4b9efb2f-a0f3-4ea0-9328-d62dcc82dede



# note
## new
`NeoPixel.new` can accept one or two arguments. The first argument specify the number of leds. The optional second argument means the GPIO number connected to leds. The default value of GPIO number is PortA-Pin2 (for Core2, it is 32).
## map1, map3
Instances of NeoPixel class have `map1` and `map3` methods to accept integer Array (of Array) to set led colors.

map1 accepts one array of arrays such as `[ [r,g,b], [r,g,b], ... ]`

map3 accepts three arrays such as [r1,r2,r3,...], [g1,g2,g3,...], [b1,b2,b3,...].

The signature of the methds are:
- map1(ary_rgb, start_offset=0)
- map3(ary_r,ary_b,ary_b, start_offset=0)

## class reference
[Classes.md](Classes.md)
