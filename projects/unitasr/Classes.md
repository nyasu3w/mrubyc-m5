# Class Reference 

Note that **mruby/c does not distingush between class methods and instance methods.**

## Implemented classes

### UnitASR (c_unitasr.cpp)
A class to use UnitASR. Assuming UnitASR is connected to PortA (hard corded in initialize_unitasr()).

- receive(timeout=0)
  > receive recoginized command from UnitASR. If timeout is set, the method will wait for the specified time (in milliseconds) for a command. 
  The received command is returned as an Integer. If no command is received within the timeout, nil is returned. If a command is receiving in the middle, false is returned.
- command_str(command)
  > Converts the command number to a string. The command number is the value returned by the receive() method. The command string is returned as a String. If the command number is not valid, "" is returned.
