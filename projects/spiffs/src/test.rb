file = "/test.txt"
png = "/test.png"
if SPIFFS.available?
  Display.puts "SPIFFS is available"
  Display.puts "files in / :"
  d=SPIFFS.open("/")
  while f=d.open_next_file do
    Display.puts f.path
  end
  d.close
  Display.puts "----"

  if SPIFFS.exists?(file) then
    Display.puts "File #{file} exists"
    txt = SPIFFS.open(file)
    Display.puts "the contents:"
    Display.puts txt.read(20)
    txt.close
    Display.puts "---\nFile #{file} closed"
  else
    Display.puts "File #{file} does not exist"
  end
  Display.puts "Hit BtnA or BtnB"

  while ! (BtnA.was_pressed? or BtnB.was_pressed?) do
    sleep 0.2
    M5.update
  end

  if SPIFFS.exists?(png) then
    Display.clear
    Display.draw_pngfile(SPIFFS,png,10,10)  # SPIFFS can be SD to use SDcard
  end
  Display.set_cursor 0,220
  Display.puts "Hit BtnA to restart"

  M5.update
  sleep 0.5
  while !BtnA.was_pressed? do
    sleep 0.2
    M5.update
  end
  exit  #restart
  
else
  Display.puts "SPIFFS is not available"
end
while true do sleep 1 end
