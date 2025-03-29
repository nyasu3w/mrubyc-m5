
while true do
  a = UnitASR.receive(500)
  if a then
    puts "command: #{a.inspect} = #{UnitASR.command_str(a)}"
    Display.puts "command: #{a.inspect} = #{UnitASR.command_str(a)}"
  end
end
