
# global settings
def hit_any_key
  M5.update
  return (BtnA.is_pressed? or BtnB.is_pressed? or (CardKB.available? and CardKB.getch!=0) or (Touch.count>0 and Touch.was_clicked?(0)) )
end

#Display.clear
#Display.draw_bmp(SD.open("/mrubyc/ruby.bmp"),0,0)
#Display.draw_png(SD.open("/mrubyc/ruby.png"),40,0)
#Display.draw_jpg(SD.open("/mrubyc/ruby.jpg"),80,0)
#while !hit_any_key do sleep 0.1 end

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
boards=[
  "displayless","M5Stack","M5StackCore2","M5StickC","M5StickCPlus",
  "M5StickCPlus2","M5StackCoreInk","M5Paper","M5Tough","M5Station",
  "M5StackCoreS3","M5AtomS3","M5Dial","M5DinMeter","M5Cardputer",
  "M5AirQ","M5VAMeter","M5StackCoreS3SE","M5AtomS3R",
]

#
puts "Hello world"

board=M5.board
if board>127 then board=0 end
puts "Board: #{boards[board]}(#{board})"
Display.clear

if Speaker then
  Speaker.volume = 20
  Speaker.tone 1000,100,0
  Speaker.tone 2000,100,0,false
end

Display.set_text_size(1) if Display.dimension[0]<240
Display.set_cursor(0,0)
Display.puts "Board: #{boards[board]}(#{board})"
Display.puts "Display: #{Display.dimension.inspect}"
Display.puts "Touch: " + ((Touch.available?)? "OK":"NG")
Display.puts "Imu: " + ((IMU.available?)? "OK":"NG")
Display.puts "SD:" + ((SD.available?)? "OK":"NG")
Display.puts "CardKB:" + ((CardKB.available?)? "0x#{CardKB.addr.to_s(16)}" :"NG")
Display.puts

cur=Display.get_cursor
cur[0]+=12 if Display.dimension[0]>300
Display.set_cursor(*cur)
Display.set_text_size(4) if Display.dimension[0]>300
[["H",white,red],["e",white,green],["l",white,blue],["l",white,purple],["o",white,cyan], [" ",white,black], 
 ["W",white,gray], ["o",white,darkgray], ["r",white,lightgray], ["l",white,white], ["d",white,black], ["!",white,red], ["\n",white,black]
].each do |c|
  Display.set_text_color(c[1],c[2])
  Display.print c[0]
end

Display.set_text_size(2);
Display.set_text_color(0xffff)

Display.puts
Display.puts "Hit any key"
while !hit_any_key do sleep 0.1 end
  Display.puts "!"


  
xscale=2.0
yscale=2.0
xoffset=0
yoffset=50
case board 
when 3,4,5 # M5StickC
  xscale=0.6
  yscale=0.6
  xoffset=0
  yoffset=80
when 11 #atomS3
  xscale=0.8
  yscale=0.8
  yoffset=50
end

if board>0 then
  puts "Mexican Hat"
  Display.clear
  Display.puts "Mexican Hat"
  def dot(x,y,c)
    Display.draw_line(x.to_i,y.to_i,x.to_i,y.to_i,c)
  end

  d=Array.new(160,100)

  dr=3.141592/180
  (-30..30).each do |by|
    (-30..30).each do |bx|
      x=bx*6; y=by*6
      r=dr*Math.sqrt(x*x+y*y)
      z=100*Math.cos(r) - 30*Math.cos(3*r)
      sx=(80+x/3-y/6).to_i
      sy=(40-y/6-z/4).to_i
      if sx>=0 && x<160 then
        if d[sx]>sy then
          zz=((z+100)*0.035).to_i+1
          if [1,2,5,7].include?(zz) then
            dot(xscale*sx+xoffset,yscale*sy+yoffset,white)
          elsif  [2,3].include?(zz) or zz>=6 then
            dot(xscale*sx+xoffset,yscale*sy+yoffset,green)
          elsif zz>=4 then
            dot(xscale*sx+xoffset,yscale*sy+yoffset,purple)
          end
          d[sx]=sy
        end
      end
    end
  end
  Display.puts "Hit any key"
  while !hit_any_key do sleep 0.1 end
end
Display.puts "!"
sleep 0.5

if Touch.available? then
  puts "touch"
  Display.clear
  Display.puts "Touch to draw lines"
  while !hit_any_key do 
    (0..(Touch.count-1)).each do |i|
      d = Touch.detail(i)
      Display.draw_line(d[0],d[1],d[2],d[3],white)
#      puts "#{i}: #{d.inspect}"
    end
  end
  Display.puts "!"
  sleep 0.5
end

if IMU.available? then
  puts "IMU"
Display.clear
  Display.puts "The Circle moves by the accelerometer"
  Display.puts "Hit any key to continue"
  c=0
  w=0xffff
  g=Display.color565(128,128,128)
  while !hit_any_key do
    a=IMU.accel
    Display.fill_circle((80*xscale+a[0]*50*xscale).to_i,(80*yscale+a[1]*50*yscale).to_i,10,w)
    c+=1
    sleep 0.01
    Display.fill_circle((80*xscale+a[0]*50*xscale).to_i,(80*yscale+a[1]*50*yscale).to_i,10,g)
    M5.update
  end
  Display.puts "!"
  sleep 0.5
end

if SD.available? then
  puts "SD"
  Display.clear
  Display.puts "SD Card test"

  def sd_read_test(fname)
    Display.print( "#{fname} exists?: ", SD.exists?(fname) )
    Display.puts
    if SD.exists?(fname) then
      f=SD.open(fname,"r") 
      Display.puts "18c: #{f.read(18)}"
      f.close
      Display.puts
    end
  end

  if SD.exists?("/nofile.txt") then
    Display.puts "nofile.txt removed"
    SD.remove("/nofile.txt")
  end
  Display.puts "creating nofile.txt"
  f=SD.open("/nofile.txt","w")
  f.write("Hello SD card!\n")
  f.close

  sd_read_test("/apikey.txt")
  sd_read_test("/nofile.txt")

  sleep 1
  Display.puts "","Hit any key"
  while !hit_any_key do sleep 0.1 end
  Display.puts "!"
  sleep 0.5
end

puts "CardKB"
Display.set_text_color(0xffff)
Display.clear
Display.puts "Input test. CardKB/BtnA,B","BtnC to end"
while true do
  M5.update
  if BtnC.is_pressed? then
#    Display.clear
    Display.puts "\nBtnC is pressed to end"
    sleep 1
    raise "end"
  end

  if BtnA.is_pressed? then
    Display.print "[A]"
  elsif BtnA.was_pressed? then
    Display.print "[a]"
  end

  if BtnB.is_pressed? then
    Display.print "[B]"
  elsif BtnB.was_pressed? then
    Display.print "[b]"
  end

  k= CardKB.getch
  if k==13 then k=10 end
  if k>0 then
    Display.print k.chr
  end

#  Display.print(".")
  sleep 0.2
end
puts "the last words in this world"