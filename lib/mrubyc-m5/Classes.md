# Class Reference 
Hopefully all implemented classes and methods are described. Many classes and methods can be disabled by C++ side configuration.
All functions shall not be implemented. It should be on demand.

Note that **mruby/c does not distingush between class methods and instance methods.**

## Implemented classes as default

### M5  (c_m5.cpp)
Implemented uncategorized features of M5Unified.
- board
  > Representating M5.getBoard(). Returns board number defined in [M5GFX](https://github.com/m5stack/M5GFX/blob/master/src/lgfx/boards.hpp).
- update
  > Representating M5.update(). If you call this methods multiple times within 50ms, it is ignored.
- display_counts  \[USE_MULTIDISPLAY\]
  > Returns how many displays/LCDs are controlled by M5Unified  
- random
  > Temporarly implemented random function, because I cannot find random function in mruby/c.
  > Representing arduino's random(max) or random(min,max)
- randomseed(seed)
  > Sets random seed for the above random() methods.

### FailObject (c_m5.cpp)
All methods return false. When SD is not available in the environment, `SD` constant is a FailObject.

### BtnClass (c_display_button.cpp)
Constants BtnA, BtnB and BtnC are already prepared. No need to think about the class.Use like BtnA.is_pressed? 

Constant BtnEXT and BtnPWR can be available if USE_FULL_BUTTONS is set in C++ side configuration.

Note that M5.update() is necessary to update the button's status.

- is_pressed?
  > check if the button is pressed. Representing BtnX.is_pressed()
- was_pressed?
  > check if the button was pressed. Representing BtnX.was_pressed()


### Display (c_display_button.cpp) \[USE_DISPLAY_GRAPHICS\]
Representing M5.Display.
- available?
  > check if Display available
- print(args...)
  > Prints all arguments with M5.Display.print()
- println(args...)
- puts(args...)
  > Prints all arguments with new lines.
  > When no args are given, just makes a new line.
- set_text_size(size)
  > Representing M5.Display.setTextSize(). Setting text size printed on M5.Display. 1<=size<=7.
- set_text_color(color)
  > Representing M5.Display.setTextColor(). Setting text color. Note that the color is RGB565 16bit value.
  > color565 methods can convert 3x8bit RGB color to 16bit.
- clear(color=0)
  > Representing M5.Display.clear(color). Clears display with the color in RGB565.
  > Additionally, the cursor position is reset to (0,0)
- set_cursor(x,y)
  > Representing M5.Display.setCursor(x,y)
- get_cursor
  > Returns the cursor position in \[x,y\]. Representing M5.Display.getCursorX/Y()
- color565(R,G,B)
  > Converts 3x8bit RGB value to 16bit RGB565 value.
- dimension
  > Returns screen dimension in \[widht, height\] of M5.Display,

\[USE_DISPLAY_GRAPHICS\]

- draw_rect(x,y,w,h,color)
  > draw a rectangle
- fill_rect(x,y,w,h,color)
  > draw a filled rectangle
- draw_circle(x,y,r,color)
  > draw a circle
- draw_circle(x,y,r,color)
  > draw a filled circle
- draw_line(x1,y1,x2,y2,color)
  > draw a line
- draw_bmpfile(picfile,x,y)
- draw_jpgfile(picfile,x,y)
- draw_pngfile(picfile,x,y)
  > draw a picture at (x,y). `picfile` is a File object, such as SD.open("/test.jpg")
- draw_bmpstr(picstr,x,y)
- draw_jpgstr(picstr,x,y)
- draw_pngstr(picstr,x,y)
  > draw a picture at (x,y). `picstr` is a String that has picture file contents
- set_font(font)
  > Sets font of drawing texts. `font` can be eigher an integer or a string.
  > The string parameter is a name of the fonts. Integer parameter is just an old interface.
  > If giving 0 as `font`, it resets to the default font.
- scroll(dx,dy)
  > Scrolls/Shift the contents. The delta is (dx,dy)

\[USE_MULTIDISPLAY\]

- change(no)
  > Changes primary display to number`no`.
  > `Display` controls the primary display.
- get_index
  > Currently selected display number
- device_type( n= *the primary display* )
  > Returns the device type of the display.
  > In case of 1 display board, it is same as `M5.board`. See its description.

### File (c_file.cpp) \[USE_FILE_CLASS\]
Representing a File class of Arduino. The instance is created by another class (ex: SD.open())
When the object is not necessary, call `destroy` method to free its memory. The destroyed object is not valid.

- read(len=255)
  > Returns a String read from the file. `len` can be specified to limit the bytes to be read.
- write(string)
  > Writes `string` to the file
- close
  > Closes the file.
- available?
  > Checks if the file can be read more.
- size
  > Returns the file size.
- name
  > Returns the file name.
- path
  > Returns the file path.
- is_directory?
  > Checks if it is a directory. When it is a directory, `open_next_file` method is available.
- open_next_file
  > Returns a new File instance of the next entry in the directory.
  > Representing arduino's File::openNextFile()
- destroy
  > Destroys the object to free memory.

### IMU (c_imu.cpp) \[USE_M5UNIFIED_IMU\]
Representing M5.Imu.

- available?
  > Checks its availability
- accel
  > Representing M5.Imu.getAccel().
  > The return value is in \[accX, accY, accZ\].
- gyro
  > Representing M5.Imu.getGyro().
  > The return value is in \[gyrX, gyrY, gyrZ\].
- temp
  > Representing M5.Imu.getTemp().
  > The return value is the temperature

### SD (c_sd.cpp) \[USE_SD_FUNCTION\]
Representing arduino's SD class. When SD is not available, Constant SD is a FailObject.
If you have compatibility issues, please adjust them yourself.

- open(filename, mode="r")
  > Opens the `filename` in mode `mode` and returns a File object.
  > `mode` is one in "r", "w" and "a".
- exists?(filename)
  > Check if the `filename` exists.
- remove(filename)
  > Removes `filename` from the SD card.

### Speaker (c_speaker.cpp) \[USE_SPEAKER\]
Representing M5.Speaker.
- tone(freq,duration,channel=-1,stop_current=true)
  > Representing M5.Speaker.tone().
  > Outputs sound of `freq`Hz in `duration`ms.
- stop(channel=-1)
  > Stops the tone. Representing M5.Speaker.stop()
- set_volume(vol)
- vol=_vol_
  > Sets volume. Representing M5.Speaker.setVolume(). (0-255)
- get_volume()
- volume
  > Gets volume. Representing M5.Speaker.getVolume()

### Touch (c_touch.cpp) \[USE_TOUCH\]
Representing M5.Touch.

- available?
  > Checks if Touch device is available.
- count
  > Returns touched counts. Representing M5.Touch.getCount()
- detail(no)
  > Representing shortly M5.Touch.getDetail(`no`)
  > Returns an array of [x, y, prev_x, prev_y, id]
- was_clicked?(no=0)
  > Checks if the touch operation was clicked.
- is_pressed?(no=0)
  > Checks if the touch operation is being pressed.
- is_released?(no=0)
  > Checks if the touch operation is being released.
- is_holding?(no=0)
  > Checks if the touch operation is being holding.

### Canvas (c_canvas.cpp) \[USE_CANVAS\]
For utilizing Sprite. 
When the object is not necessary, call `destroy` method to free its memory. The destroyed object is not valid.

The many drawing methods of Display class are also available for Canvas.
- new(width,height,color_depth)
  > Create a new Canvas (Sprite) 
- push_sprite(x,y)
- push_sprite(dst_canvas, x, y)
  > Draws the contents of the sprite on Display at ('x','y')
  > If canvas parameter is given, Draws on the `dst_canvas`
- delete_sprite()
  > after calling this method, drawing methods will not work.
- create_sprite(width, height)
  > Create Sprite
- destroy()
  > Deletes the inner object to its free memory. Called instance is no longer valid.

### Font (c_font.cpp) \[USE_DISPLAY_GRAPHICS\]
A class to calculate around Font.
To support another font, call `c_add_font(name,lgfx_font)` to register the font. This function is declared in `c_font.h`

- count
  > Returns the number of registered fonts.
- names
  > Returns an array of registered font names.
- by_name(fontname)
  > Gets the font number to give `set_font()`
  > This is an old interface to calculate font number.


### EspNow (c_espnow.cpp) \[USE_ESPNOW\]
A class to use ESP-NOW.

Wrapping rather thick than the other classes.
A sample is stored in projects/sample_rb/espnow.rb

- begin
  > Initializes to use ESP Now.
- add_peer(devinfo)
  > Opens connection to a device only this side. It means enabling of accepting a packet from the device and sending a packet to the device.
  > `devinfo` is an array as \[channel, \[mac-address-octets\]\].
  > For example, to open connection with a device with mac address 12-34-56-78-9a-bc in WiFi channel 3, `devinfo` is \[3,\[0x12,0x34,0x56,0x78,0x9a,0xbc\]\].
  > Broadcasting is supported by mac address ff-ff-ff-ff-ff-ff. For now, only 4 devices (incl. broadcasting) can be paired. 
  > The return value is an integer to handle connection and target of sending data.
- del_peer(dev_no)
  > Closes connection only this side. `dev_no` is a return value of `add_peer`
- send(dev_no, data)
  > Sends `data` to a device of `dev_no`.
- last_send_status
  > true or false. The status of the last call of `send`, but it can be jammed.
- received
  > number of stocked incomming packet. For now, 8 packets can be stocked.
- recv(rtype)
  > Reads 1 packet data.
  > When `rtype` is not given or `rtype==0', then returns packet data as String.
  > When `rtype==1` then returns an array of \[data, \[sender-mac-addr\]\].
  >For another `rtype`, returns false.
  > If no packet can be read, returns nil
- policy
- get_policy
  > Returns true or false. This indicates stocking policy of incoming packets.
  > If true, the incoming packets are stored in ring buffer. old one is overwritten by new one when the buffer is full.
  > If false, an overflow packets are thrown and old data are kept.
- policy=
- set_policy(bool_value)
  > Sets the policy described above.

## Implemented classes in sample projects
To use these classes, copy the c_XXX.h and c_XXX.cpp to your project src directory, and call class_XXXX_init() function after my_mrubyc_init().

- [M5Dial](../../projects/m5dial/Classes.md)
- [M5Stack-Avatar](../../projects/m5stack-avatar/Classes.md)
- [NeoPixel](../../projects/neopixel/Classes.md)
- [SPIFFS](../../projects/spiffs/Classes.md)
- [Wifi, HTTPClient](../../projects/wifi/Classes.md)
- [CardKB](../../projects/cardkb/Classes.md)





