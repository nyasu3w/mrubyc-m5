
Display.set_text_size(2)
Display.clear
Display.puts
Display.puts "       M5Dial"
puts "M5Dial"
old_pos = -12345
Display.set_text_size(4)
while true do
  M5.update
  if BtnA.was_pressed then
    puts "BtnA was pressed"
    Dial.read_and_reset(0)
  end

  pos=Dial.read
  
  if pos != old_pos then
    s_pos = pos.to_s
    Display.fill_rect(0,80,240,120,0)  # clear the area
    Display.set_cursor(120-10*s_pos.size,100) # center the text
    Display.print s_pos
    old_pos = pos
  end
end

