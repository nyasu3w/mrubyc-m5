
# For NECO Unit
n=70

led = NeoPixel.new n,32  # 4 neopixels on pin32

while true do
  Display.clear
  Display.set_text_size(4)
  0.upto(2) do |ly|
    ca=[0,0,0] 
    ca[ly]=255
    Display.set_text_color Display.color565(*ca)
    Display.puts "Color: " + ["RED","GREEN","BLUE"][ly]

    0.upto(256*2) do |br|
      c=(br<256)? br : 512-br  # triangle wave
      ca=[0,0,0]
      ca[ly]=c
      0.upto(n-1) do |i|
        led.set_pixel_color(i,*ca)
      end
      led.show
    end
  end
  led.clear
  Display.set_text_color 0xffff
  Display.puts "cleared"
  sleep 3
end
  


