# Class Reference 
To use encoder of M5Dial.

Note that **mruby/c does not distingush between class methods and instance methods.**

## Implemented classes

### Dial (c_m5dial.cpp)
M5.update is necessary to update the status.

- read
  > Reads the value of Encoder.
  > Representing M5Dial.Encoder.read().
- write(pos)
  > Sets the value of Encoder,
  > Representing M5Dial.Encoder.write(pos).
- read_and_reset
  > Read the Encoder value and reset Encoder.
  > Representing M5Dial.Encoder.readAndReset().


