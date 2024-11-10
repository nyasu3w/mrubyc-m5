# Class Reference 
To use M5Stack CardKB

Note that **mruby/c does not distingush between class methods and instance methods.**

## Implemented classes

### CardKBClass (c_cardkb.cpp) \[USE_CARDKB in c_cardkb.h\]
A class to use M5Stack CardKB. A Constant CardKB is already prepared for default I2C address. If the address is different, you need to create a new instance for it.
If CardKB is not available, CardKB is a FailObject.

- new(addr)
  > create a new instance of CardKBClass for the card whose i2c address is `addr`
- addr
  > returns its i2c address
- getch
  > Gets a pressed key from CardKB. When 'a' key was pressed, returns the ascii code of 'a'.
  > If no key was pressed, returns 0.
- available?
  > check if the cardkb is available
