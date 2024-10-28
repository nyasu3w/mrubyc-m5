a=Avatar.new

case M5.board
when 3 # M5StickC
  a.scale=0.3
  a.set_position(-30,-120)
when 11 # AtomS3
  a.scale=0.4
  a.set_position(-55,-100)
end
a.start
sleep 2
exp=[ "Happy", "Angry", "Sad", "Doubt", "Sleepy", "Neutral"]

def check_touched(avatar)
  if Touch.is_pressed?(0) then
    avatar.set_speech_text("何でしょう")
  end
end

while true do
  (0..18).each do |i|
    a.set_rotation(i*20)
    sleep 0.1
  end
  (0..(exp.size-1)).each do |i|
    a.set_speech_text("")
    a.set_expression(i)
    sleep 0.5
    a.set_speech_text("私は #{exp[i]}")
    count=0
    while count<40 do
      M5.update
      check_touched(a)
      sleep 0.1
      count+=1
    end
  end
  a.set_speech_text("")
end