
Display.puts "CardKB:" + ((CardKB.available?)? "0x#{CardKB.addr.to_s(16)}" :"NG")
Display.puts "Press keys. BtnA to end"
while true do
  M5.update

  k= CardKB.getch
  if k==13 then k=10 end
  if k>0 then
    Display.print k.chr
  end

  if BtnA.is_pressed? then
#    Display.clear
    Display.puts "\nBtnA is pressed to end"
    sleep 1
    raise "end"
  end

  if BtnB.is_pressed? then
    Display.print "[B]"
  elsif BtnB.was_pressed? then
    Display.print "[b]"
  end

  if BtnC.is_pressed? then
    Display.print "[C]"
  elsif BtnA.was_pressed? then
    Display.print "[c]"
  end


#  Display.print(".")
  sleep 0.2
end
