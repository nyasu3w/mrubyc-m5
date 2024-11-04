
# For NECO Unit
n=70

led = NeoPixel.new n  # strip on portA 
#led = NeoPixel.new n ,32  # 70 neopixels on pin32

M5.randomseed  # for temporal until mruby/c supports random number


while true do
  Display.clear
  Display.set_text_size(3)
  0.upto(2) do |ly|  # for each red,green,blue
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
  sleep 0.5

  Display.clear
  Display.puts "random colors"

  r=[]; g=[]; b=[]
  rgb=[]

  0.upto(10) do
    0.upto(69) do |i|
      r[i]=M5.random(256)
      g[i]=M5.random(256)
      b[i]=M5.random(256)
    end
    led.map3(r,g,b)
    led.show
    sleep 0.5

    0.upto(69) do |i|
      rgb[i] = [M5.random(256),M5.random(256),M5.random(256)]
    end
    led.map1(rgb)
    led.show
    sleep 0.5
  end

  sleep 1
end
  


