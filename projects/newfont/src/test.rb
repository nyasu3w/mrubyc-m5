
def hit_any_key
  sleep 1
  M5.update
  
  Display.set_font 0
  Display.puts
  Display.puts "Hit any key to go next"
  while true do
    M5.update
    break if (BtnA.is_pressed? or BtnB.is_pressed? or BtnC.is_pressed? or (Touch.count>0 and Touch.was_clicked?(0)) )
    sleep 0.1
  end
end

Display.puts "Font test"
Display.puts "name, AlNum Ja"
cur = Display.get_cursor
Display.draw_line 0,cur[1]+5,319,cur[1]+5,0xffff
Display.puts


Font.names.each do |name|
  Display.set_font 0
  Display.print "#{name}: "
  Display.set_font name
  Display.puts "12ABfg 日本語でおk"
end

hit_any_key

Font.names.each do |name|
  Display.clear
  Display.set_font 0
  Display.puts "#{name}: "
  Display.set_font name
  Display.puts
  Display.puts "Lazy dog is cute. Brown fox is cool. What about you?"
  Display.puts "ここではきものをぬぎます"
  Display.puts "インド人を右に10m進んでください"
  Display.puts "北1東12：知能指数は1300"
  Display.puts "もう食べられません"
  hit_any_key
end



