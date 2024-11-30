#!/usr/bin/env ruby

# This script converts a BDF font file from ISO-2022-JP to UTF-16 encoding.
#
# Usage:
#   ruby bdfenc.rb  font-jis.bdf > font-utf16.bdf

ARGF.each_line do |line|
  case line
  when /^CHARSET_REGISTRY/
    puts 'CHARSET_REGISTRY: "ISO10646"'
#  when /^CHARSET_ENCODING/
#    puts 'CHARSET_ENCODING: "1"'
#when /^STARTCHAR/
when /^ENCODING/
    s,code = line.split
    code=code.to_i

    begin
      jis_string = [0x1b,0x24,0x42,code].pack('CCCn').force_encoding('ISO-2022-JP')
      unicode = jis_string.encode('UTF-16LE')
      hex = unicode.codepoints.map{|c| c.to_s(16)}.join
    rescue
      STDERR.puts "Conversion Error: #{line}"
      hex="0"
    end
      #    puts "STARTCHAR #{hex}"
    puts "ENCODING #{hex.to_i(16)}"
  else
    puts line
  end
end