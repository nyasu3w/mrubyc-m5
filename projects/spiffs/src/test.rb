
file = "/test.txt"
if SPIFFS.available?
  Display.puts "SPIFFS is available"
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

else
  Display.puts "SPIFFS is not available"
end
while true do sleep 1 end
