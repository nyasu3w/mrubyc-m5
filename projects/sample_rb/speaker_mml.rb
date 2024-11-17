# for Core series.
e
def calc_freq(octave, note)  # almost written by copilot (except to_i)
  (case note
  when "C" then 16.35
  when "C+" then 17.32
  when "D-" then 17.32
  when "D" then 18.35
  when "D+" then 19.45
  when "E-" then 19.45
  when "E" then 20.60
  when "F" then 21.83
  when "F+" then 23.12
  when "G-" then 23.12
  when "G" then 24.50
  when "G+" then 25.96
  when "A-" then 25.96
  when "A" then 27.50
  when "A+" then 29.14
  when "B-" then 29.14
  when "B" then 30.87
  else 0
  end * (2 ** octave.to_i) ).to_i
end

def calc_duration(note,tempo) 
  (240000.0 / note.to_f / tempo.to_i).to_i
end

def separate_mml(mml)  # split /[A-Z][0-9]*.?]/
  notes=[]
  skip=0
  0.upto(mml.length-1) do |i|
    if skip>0 then
      skip-=1
    else
      c=mml[i].upcase
      case c.ord
      when 0x41..0x5a then # [A-Z]
        notes.push c 
      when 0x2b, 0x2d, 0x30..0x39,0x2e then # [+-0.9.]
        notes[-1]+=c
      end
    end
  end
  notes
end

def analyze_smml(smml)
  oc=6
  nt=4
  tm=120
  
  smml.map do |n|
    if n.length<2 or ! "+-".include?(n[1]) then
      r = [ calc_freq(oc,n[0]), (n.length>1)? n[1,n.length-1] : "", n ] # n[1..-1] 
    else
        r = [ calc_freq(oc,n[0,2]), (n.length>1)? n[2,n.length-2] : "",n ]   # n[2..-1] 
    end
    case n[0].ord
    when 0x41..0x47,0x52 then # [A-GR]
      if(r[1]=="") then 
        r[1] = calc_duration(nt,tm)
      else
        nnt=r[1].to_i
        if ".".include?(r[1][-1,1]) then
          nnt = nt 
        end
        du=calc_duration(nnt,tm)
        if r[1][-1,1]=="." then
          du = du * 1.5
        end
        r[1] = du
      end
    when 0x4f then # O
      if n[1]=="-" then
        oc-=1
      elsif n[1]=="+" then
        oc+=1
      else
        oc=r[1].to_i
      end
      r[0]=nil; r[1]=0
    when 0x54 then # T
      tm=r[1].to_i
      r[0]=nil; r[1]=0
    when 0x4c   # L
      nt=r[1].to_i
      r[0]=nil; r[1]=0
    else
      r[0]=nil; r[1]=0
    end
    r
  end
end

def play_mml(mml,ch=0)
#  prev_dur=0
  analyze_smml( separate_mml(mml) ).each do |nt|
    Display.print nt[2]
    if ! nt[0].nil? then
      if nt[0]>0 then
        Speaker.tone nt[0],nt[1],ch,false
#        prev_dur=nt[1]
      else
#        sleep (prev_dur/1000.0 - 0.01 )
        while Speaker.is_playing?(ch) do sleep 0.001 end
#        sleep nt[1].to_f / 1000.0
      end
      sleep (nt[1]/1000.0 - 0.05 )
    end
  end
 Display.puts
end

def play_mmla(*mmla)
  #  prev_dur=0
  mm=mmla.map do |mml|
    analyze_smml( separate_mml(mml) )
  end
  wait=Array.new
  while true do
    less_wait = nil
    mm.each_with_index do |mml,ch|
      nt = mml[0]
      next if nt.nil?
      wait_mil=(wait[ch]||Utils.millis)-Utils.millis
      if wait_mil < 50 then  # if next tone is within 100ms, queue it
        if ! nt[0].nil? then
          if nt[0]>0 then
              Speaker.tone nt[0],nt[1],ch,false
          end
        end
        wait[ch] = wait_mil + nt[1] + Utils.millis
        #puts "wait: ch=#{ch} #{wait[ch]} : now#{Utils.millis}"
        mml.shift
      end
      less_wait = wait[ch] if less_wait.nil? or wait[ch] < less_wait
    end

    break if less_wait.nil?
   w_mil = less_wait-Utils.millis
    sleep (w_mil-50)/1000.0 if w_mil>100
  end
end  

Speaker.volume=30

Display.puts "Chime"
play_mmla "C1","r4E2.","r2g2"

sleep 1
Display.puts "Play MML"
sleep 1

Display.print "smml:"
play_mml "cde2 cde2 gedc dedr"
sleep 1
Display.print "smml:"
play_mml "ccgg aag2 ffee ddc2",1
sleep 1
Display.print "smml:"
play_mml "g.a8gf efg2 def2 efg2"
sleep 1

Display.puts "neko"
Display.print "smml:"
play_mml "e-d-o-f+2 b-16o+f+16o-b-16o+f+16r8 o-b-16o+f+16o-b-16o+f+16r8"
sleep 1
Display.print "mmml:"
play_mmla "e-d-o-f+2 b-16o+f+16o-b-16o+f+16r8 o-b-16o+f+16o-b-16o+f+16r8"
sleep 1
Display.print "mmml(full):"
play_mmla "e-d-r2 f+rf+r","o-r2f+2 b-rb-r"
Display.puts
sleep 1

Display.puts
Display.puts "Hit BtnA to reset"
M5.update 
while ! BtnA.was_pressed? do 
  sleep 0.1; 
  M5.update 
end


