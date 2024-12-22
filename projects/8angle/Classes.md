# Class Reference 


## Implemented classes

### Angle8Class
Maybe you have no chance to write down "Angle8Class."
An enough instance is already created as "Angle8". Use this `Angle8`.

- new(i2caddr)
  > Creates a new instance, but normally this method is useless.
  > When the unit is not connected, this returns FailObject.
- dswitch
  > Returns `true` or `false` according to the status of the digital switch.
- set_led_color(n,color)
  > Sets the `color` of `n`th LED.
  > The color is 24bit value of 0xRRGGBB. The left edge is 0th LED.
- get_analog_input(n,mode=0)
  > Retrieves the position of the 'n'th volume.
  > `mode` specifies the resolution of the value. 0 is for 8bit and 1 is for 12bit.

