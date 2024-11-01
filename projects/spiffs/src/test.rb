
if SPIFFS.available?
  Display.puts "SPIFFS is available"
  if SPIFFS.exists?("/test.txt")
    Display.puts "File /test.mrb exists"
  else
    Display.puts "File /test.mrb does not exist"
  end

else
  Display.puts "SPIFFS is not available"
end
while true do sleep 1 end
