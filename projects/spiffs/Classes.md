# Class Reference 
Note that **mruby/c does not distingush between class methods and instance methods.**

## Implemented classes

### SPIFFS (c_spiffs.cpp)
A class to use SPIFFS. Almost same as SD class.
Representing arduino's SPIFFS class. When SPIFFS is not available, Constant SPIFFS is a FailObject.

- open(filename, mode="r")
  > Opens the `filename` in mode `mode` and returns a File object.
  > `mode` is one in "r", "w" and "a".
- exists?(filename)
  > Check if the `filename` exists.
- remove(filename)
  > Removes `filename` from the SPIFFS.
