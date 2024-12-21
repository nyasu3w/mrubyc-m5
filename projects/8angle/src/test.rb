
colors=[ 0x0000FF, 0x00FF00, 0xFF0000, 0xFFFF00, 0xFF00FF, 0x00FFFF, 0xFFFFFF, 0x000000, 0x808080 ]

Display.clear
Display.puts "8Angle"

canvas = Canvas.new(320,160)
canvas.set_text_size(2)
millis=0

while(true) do
  canvas.clear

  canvas.print("SWITCH: ")
  if Angle8.dswitch then
    canvas.set_text_color(0,Display.color565(255,255,0))
    canvas.puts " ON "
  else
    canvas.set_text_color(0xffff,0)
    canvas.puts " OFF "
  end

  canvas.set_text_color(0xffff)

  (0..7).each do |i|
    Angle8.set_led_color(i, colors[i])
    canvas.print "LED#{i}:"
    x,y = canvas.get_cursor
    canvas.puts
    val = 255 - Angle8.get_analog_input(i)   
    canvas.draw_rect(x,y,255,10,0xffff)
    canvas.fill_rect(x,y,val,10,0xffff)
  end
  Angle8.set_led_color(8, colors[8])

  Display.wait_display
  canvas.push_sprite(0,20)

  if Utils.millis - millis > 1000 then
    a=colors.shift
    colors.push(a)
    millis=Utils.millis
  end
#  sleep 0.5
end


