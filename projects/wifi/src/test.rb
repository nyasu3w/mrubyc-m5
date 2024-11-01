wifimode=[
  :WIFI_MODE_NULL,
  :WIFI_MODE_STA,
  :WIFI_MODE_AP,
  :WIFI_MODE_APSTA,
]
wl_status = [
  :WL_IDLE_STATUS,
  :WL_NO_SSID_AVAIL,
  :WL_SCAN_COMPLETED,
  :WL_CONNECTED,
  :WL_CONNECT_FAILED,
  :WL_CONNECTION_LOST,
  :WL_DISCONNECTED,
#  :WL_NO_SHIELD, = 255
]

Display.puts "Wifi connect test"
Wifi.mode = wifimode.index(:WIFI_MODE_STA)
Wifi.begin # or Wifi.begin("SSID","PASSWORD")

r = Wifi.wait_for_connect

if r==wl_status.index(:WL_CONNECTED) then
  Display.puts "OK: STA mode connected"
else
  Display.puts "NG: STA mode failed(#{wl_status[r]})"
end

Display.puts "start to conenct"
# Note: google returns too big response to make exception
hc=HTTPClient.new("http://www.example.com/")   #not for https
rc = hc.get
Display.puts "HTTP GET: #{rc}"
resp = hc.response
Display.puts "HTTP Response: #{resp}"
hc.close

sleep 10
Wifi.disconnect 
Display.puts "disconnected"

while(true) do sleep 1 end
