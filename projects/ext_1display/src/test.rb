
# global settings
def hit_any_key
  M5.update
  return (BtnA.is_pressed? or BtnB.is_pressed? or (Touch.count>0 and Touch.was_clicked?(0)) )
end

def draw_on(disp)
  prev=Display.get_index
  Display.change(disp)
  yield if block_given?
  Display.change(prev)
end


boards=[
  "displayless","M5Stack","M5StackCore2","M5StickC","M5StickCPlus",
  "M5StickCPlus2","M5StackCoreInk","M5Paper","M5Tough","M5Station",
  "M5StackCoreS3","M5AtomS3","M5Dial","M5DinMeter","M5Cardputer",
  "M5AirQ","M5VAMeter","M5StackCoreS3SE","M5AtomS3R",
]

board = M5.board

if Speaker then
  Speaker.volume = 20
  Speaker.tone 1000,100,0
  Speaker.tone 2000,100,0,false
end

0.upto(M5.display_counts-1) do |i|
  draw_on(i) do
    Display.clear
    dimension = Display.dimension  # [w,h]
    Display.set_rotation(1) if dimension[0]<dimension[1]
    Display.set_text_size(1) if Display.dimension[0]<240

    Display.set_text_color(0xffe0,0)
    Display.puts "Board: #{boards[board]}(#{board})"
    Display.puts "Display(s): #{M5.display_counts}"
    Display.set_text_color(0xffff,0)
    Display.puts "Display: #{i}"
    Display.puts "Type: #{Display.device_type}"
  end
end

Display.puts "Hit any key to restart"
while !hit_any_key do sleep 0.05 end
Display.puts "!"

Display.clear
Display.puts "This VM will finish in 3 seconds"
sleep 3
puts "the last words in this world"