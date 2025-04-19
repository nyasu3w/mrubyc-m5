
white=0xffff
blue=0x001f
green=0x07e0
purple=0xf81f
red=0xf800
cyan=0x07ff
yellow=0xffe0
black=0x0000
gray=0x8410
darkgray=0x4208
lightgray=0xc618

encoder=4
current=3
position=2
speed=1

puts Roller485.inspect
if Roller485.is_a?(FailObject) then
  while true do
    puts "Roller485 false"
    sleep 2
  end
end


Roller485.set_rgb_mode 1
Roller485.set_rgb purple
Roller485.calibrate

while true do
  puts "encoder"
  Roller485.set_rgb green 
  sleep 1
  Roller485.set_mode(encoder);
#  Roller485.set_current(120000);
  Roller485.set_output(1);
  sleep 2

  Roller485.set_output 0
  puts "speed"
  Roller485.set_rgb red
  sleep 1
  Roller485.set_mode speed
  Roller485.set_output(1)
  Roller485.set_speed 80000
#  Roller485.set_speed_maxcurrent(100000)
  sleep 2
  Roller485.set_rgb 0
  Roller485.set_output(0)

  puts "current"
  Roller485.set_output(0);
  Roller485.set_rgb yellow
  sleep 1
  Roller485.set_mode(current)
  Roller485.set_current(120000)
  Roller485.set_output(1)
  sleep 0.1
  puts "current1 #{Roller485.get_current}"
  sleep 0.5
  puts "current2 #{Roller485.get_current}"
  sleep 2

  Roller485.set_output 0
  sleep 1
  Roller485.set_rgb blue
  Roller485.set_mode position
  Roller485.set_pos 720
#  Roller485.set_pos_maxcurrent 100000
  Roller485.set_output 1
  sleep 2

end


