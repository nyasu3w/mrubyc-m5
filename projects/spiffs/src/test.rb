file = "/test.txt"
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

else
  Display.puts "SPIFFS is not available"
end
while true do sleep 1 end
