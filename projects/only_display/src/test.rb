puts "Hello world"

boards=[
  "displayless","M5Stack","M5StackCore2","M5StickC","M5StickCPlus",
  "M5StickCPlus2","M5StackCoreInk","M5Paper","M5Tough","M5Station",
  "M5StackCoreS3","M5AtomS3","M5Dial","M5DinMeter","M5Cardputer",
  "M5AirQ","M5VAMeter","M5StackCoreS3SE","M5AtomS3R",
]

board=M5.board
if board>127 then board=0 end
puts "Board: #{boards[board]}(#{board})"
Display.clear
Display.set_text_color(Display.color565(0,0xff,0))
Display.set_cursor(0,0)
Display.puts "Board: #{boards[board]}(#{board})"
Display.puts "Display: #{Display.dimension.inspect}"
Display.puts "Touch: " + ((Touch.available?)? "OK":"NG")
Display.puts "Imu: " + ((IMU.available?)? "OK":"NG")
Display.puts "SD:" + ((SD.available?)? "OK":"NG")
Display.puts "CardKB:" + ((CardKB.available?)? "0x#{CardKB.addr.to_s(16)}" :"NG")
Display.puts

sleep 100000