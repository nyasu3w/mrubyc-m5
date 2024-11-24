
# global settings
def hit_any_key
  M5.update
  return (BtnA.is_pressed? or BtnB.is_pressed? or (Touch.count>0 and Touch.was_clicked?(0)) )
end

#Display.clear
#Display.draw_bmpfile(SD.open("/mrubyc/ruby.bmp"),0,0)
#Display.draw_pngfile(SD.open("/mrubyc/ruby.png"),40,0)
#Display.draw_jpgfile(SD.open("/mrubyc/ruby.jpg"),80,0)
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
Display.puts "M5U: " + M5.unified_version
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
while !hit_any_key do sleep 0.05 end
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
  while !hit_any_key do sleep 0.05 end
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
    M5.update

    if temp=IMU.temp then
      Display.set_cursor(0,70)
      Display.set_text_color(0xffff,0)
      Display.print "temp: #{temp}"
    end

    a=IMU.gyro
    Display.fill_circle((80*xscale+a[0]*0.5*xscale).to_i,(80*yscale+a[1]*0.5*yscale).to_i,10,cyan)
    c+=1
    sleep 0.01
    Display.fill_circle((80*xscale+a[0]*0.5*xscale).to_i,(80*yscale+a[1]*0.5*yscale).to_i,10,blue)

    a=IMU.accel
    Display.fill_circle((80*xscale+a[0]*50*xscale).to_i,(80*yscale+a[1]*50*yscale).to_i,10,w)
    c+=1
    sleep 0.01
    Display.fill_circle((80*xscale+a[0]*50*xscale).to_i,(80*yscale+a[1]*50*yscale).to_i,10,g)
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
  while !hit_any_key do sleep 0.05 end
  Display.puts "!"
  sleep 0.5
end


puts "Sprite"

### picture load section
mrubycb64=
"iVBORw0KGgoAAAANSUhEUgAAABkAAAAZCAIAAABLixI0AAAABnRSTlMAgAAAAP/yQliOAAAACXBI
WXMAAAsTAAALEwEAmpwYAAADjElEQVQ4jbVUz2tcVRQ+5/56782bmUwmsc4wNk2TNDUFMRYKgj9w
JbWLqFD/AJciBZVu3Lh16UZdCIK4U9AK1Y0FRaRVsKVSwcbYQmPEGNPJ/J55990fx8W0ycx0KAT0
43Hhu4fv49zv3XMB/jvgvVtLMloJi2MKAABQNjL27IJvfGKrIyUxwgtMnM1WishnjNoSNkE/WH3A
ykMggcFjKr6W9K7b7mCVDTeJZ7KlIorDaVC0QiOFxJZ0GBIDgLznM0YCQIf7DWVez5ZzjA/Kh8iL
0dSpYHI+VZOW14XjBAtpEHhsCoeAizpkBCmjtVA7oAyyCg8ups0xXosiejOuPKzDrGP9Hqet6KfW
Y3TQKEnoENZCrZH6kgpXbfK/22TIK0L2XubwsslIuhO6oL30c54LQkC4GaRt5gaP8ojMXDbtund7
2U+AOGajofDuYj1Im8wDwENGzqQSQA5WCeAQBbdA72X/N6Wn07VNMiNGfymzza1GX3B80nLlcfAj
D2/o9e/cnciGrlEJ1Udq7hhGfVoV7pbSBFDw4qCWVbJt8gjUl3XAv2X+vOo7u/LRK5lF/r6cfYbl
29yvBdoDZYgtJMENSr6y9VXqeQABaICu+k6N7KCWj3ilQOd9vcCEyjAHJAmP6HCbzOe2doEaz2Um
F1U0q8KSlJdMKyW6nxcAeIBvXTMBWpbxUR2gx59851O3sxIXj8t4molpJspclbm6mLYGhaMztItz
vWrXuHfFLAAEgAXk05oftxkAuBHospG/pN0RyZi+dnHTJz/49kk+UUKZQ/622ayRe5Llatx9kPzz
jtm8j3Y8ZjH4UM0/wXN9eopPnuSFfbv8L0BAgeMfNIV7wzJ2bEZxVIYrYfHe/QkmzsSlXco54uMq
N8VkgYkCEzVvF0W0IKLb3laE6pCfEyFHnOdhg1zd2yMi2rm7vhAWr5hOF3yJy4Z3PM/Eq3EpRL6s
4kdlvOp6r8SlqrdPB/k5Ea5b/XLmwK+293xUzCEvcbUsY0A4HU1dStsvRVPnde1stvwgk6Z/xnWn
f0xb10z3uu1NoOiS+zLZ+SyppkQlLmPkALDlzM+me4DLc0n1tWz566T+VJD7Pm1mEFvkr5iOJs8J
QCL7w+kEqOndljeKsd9sr0t+w+lnw8KG16s2KTKRRfZFr9Yid0LlPu5tL8noG91seOcAZnhw2bT3
9T8BAPKMn1DZfcv2i38BL1aPc7z7QcYAAAAASUVORK5CYII="


def b64c(ary)
  c=ary.shift
  case c
  when 0x41..0x5a
    r=c-0x41
  when 0x61..0x7a
    r=c-0x61+0b011010
  when 0x30..0x39
    r=c-0x30+0b110100
  when 0x2b
    r=0b111110
  when 0x2f
    r=0b111111
  when 0x3d,nil
    r=-1
  else # ignore
    r = b64c(ary)
  end
end

def b64decode(str)
  b=str.bytes
  r = ""
  while b.size>3 do
    b1=b64c(b)
    b2=b64c(b)
    b3=b64c(b)
    b4=b64c(b)
    if b2<0 then
      r+= (b1*4).chr
    else
      r+= ((b1*4) + (b2/16)).chr 
    end
    r+= (((b2&15)*16) + (b3/4)).chr if b3>=0
    r+= (((b3&3)*64) + b4).chr if b4>=0
  end
  r
end

mrubycpng=b64decode(mrubycb64)
### end of picture load section

Display.clear
Display.puts "Sprite test"
Display.draw_pngstr(mrubycpng,50,40*yscale)

font=Font.by_name("DejaVu12")
Display.set_font(font); Display.puts "DejaVu12"
font=Font.by_name("efontJA_12")
Display.set_font(font); Display.puts "#{font}:日本語フォント"

dimension=Display.dimension
dimension[0]*= 3
dimension[1]/= 2

begin
  canvas = Canvas.new(*dimension)
rescue Exception=>e  # retry when canvas(sprite) creation failed
  dimension[0]/=2.5
  dimension[1]=dimension[1]*2/3
  canvas = Canvas.new(*dimension)
end

canvas.fill_rect( 10,10*yscale,100,15*yscale, 0x07e0)
canvas.draw_pngstr(mrubycpng,110*xscale,10*yscale)
canvas.draw_pngstr(mrubycpng,130*xscale,10*yscale)
canvas.draw_pngstr(mrubycpng,150*xscale,10*yscale)

canvas.set_cursor(0,40)
canvas.set_text_size(2)
canvas.set_font(font)
canvas.puts "この部分はCanvasで描画しています。そしてスクロールさせています"

canvas.push_sprite(0,50*yscale)
sleep 0.5
9.upto(400) do 
  canvas.push_sprite(0,50*yscale)
  canvas.scroll(-2,0)
  Display.wait_display
  #  sleep 0.05
end
Display.puts


Display.puts "Hit any key to restart"
while !hit_any_key do sleep 0.05 end

canvas.delete_sprite
canvas.destroy

Display.clear
Display.puts "This VM will finish in 3 seconds"
sleep 3
puts "the last words in this world"