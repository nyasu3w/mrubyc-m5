# Class Reference 
To use WiFi and HTTPClient.

Note that **mruby/c does not distingush between class methods and instance methods.**

## Implemented classes

### Wifi (c_wifi.cpp)
Representing arduino's WiFi class. Misspelling lower case 'f'.

- begin(ssid,pass)
- connect(ssid,pass)
  > Connects to WiFi network of `ssid`, or connects to the previous connected network If no arguments are given.
  > Representing WiFi.begin()
- status
  > Returns the WiFi status. Refer to `wl_status` in the sample test.rb.
- wait_for_connect(timeout=_never_)
  > Representing WiFi.waitForConnect()
- mode
  > Returns WiFi mode. Refer to `wifimode` in the sample test.rb.
  > Representing WiFi.getMode()
- set_mode(mode)
- mode= _mode_
  > Sets the WiFi mode. Station mode is default.
- disconnect
  > Disconnects from the network.
- softap(ssid, passwd, /* other args are not implemented yet */)
  > Starts accesspoint of WiFi.

### HTTPClient (c_wifi.cpp) \[USE_HTTPCLIENT in c_wifi.h\]
Representing arduino's HTTPClient class.

- new(url)
  > Connects to the `url`.
  > Representing HTTPClient.new(url)
- get
  > Gets the response for 'GET' from the host
  > Returns the status code of the reqeust.
- put(payload)
  > Gets the response for 'PUT' with `payload` from the host
  > Returns the status code of the reqeust.
- post(payload)
  > Gets the response for 'POST' with `payload` from the host
  > Returns the status code of the reqeust.
- response
  > Returns the response from the host of get/put/post request.
- size
  > Returns the length of response body.
- close
- end
  > Close the connection.

