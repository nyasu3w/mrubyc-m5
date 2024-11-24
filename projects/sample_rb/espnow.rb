=begin Example to use Esp-Now


!!! VM need to support EspNow. define USE_ESPNOW in libconfig.h.

Run this program on two devices. The devies will pair each other and send messages each other.


This sample does the following:
- paring (add_peer each other) on WiFi channel 1
  - send "request pairing" to broadcast address
  - receive "request pairing" to call add_peer(the sender). Then send "ack pairing"
  - receive "ack pairing" to finish paring
- send message to the peer 
=end

unless EspNow then
  Display.puts "EspNow is disabled"
  Display.puts "modify your main.cpp"
  sleep 10
  raise "EspNow is disabled"
  exit
end

EspNow.begin
ch=1
broadcast_addr=[0xff,0xff,0xff,0xff,0xff,0xff]
req_pairing = "request pairing"
ack_pairing = "ack pairing"

bcpeer = EspNow.add_peer([ch,broadcast_addr])
raise unless bcpeer

Display.puts "Paring"
retry_msg=req_pairing
peer=nil

while true do
  EspNow.send(bcpeer, req_pairing)
  sleep 1
  if EspNow.received>0 then
    str,mac = *EspNow.recv(1)
    if str==req_pairing then
      Display.puts "req received: #{str} from #{mac}"
      peer = EspNow.add_peer([ch,mac])
      EspNow.send(peer, ack_pairing)
      retry_msg = ack_pairing
    end
    if str==ack_pairing then
      Display.puts "ack received: #{str} from #{mac}"
      peer = EspNow.add_peer([ch,mac]) unless peer
      EspNow.send(peer, ack_pairing)
      retry_msg = ack_pairing
      break
    end
  end
  EspNow.send(bcpeer, retry_msg)
  sleep 0.2
end

Display.print "paired "
Display.puts EspNow.del_peer(bcpeer)
Display.puts "sleep 1"
sleep 1

Display.clear
Display.set_text_color(0xffff,0)

0.upto(1024) do |i|
  Display.set_cursor(0,0)
  puts EspNow.send(peer, "Hello #{i}")
  sleep 0.3
  Display.puts "Sent#{i}: #{EspNow.last_send_status} "
  sleep 0.3
  Display.puts "C: #{EspNow.received} "
  while EspNow.received>0 do
    Display.puts "recv: #{EspNow.recv} "
  end
  Display.puts "      "
  sleep 0.4
end

