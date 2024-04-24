#
# Enumerable module for mruby/c.
#
#  Copyright (C) 2015- Kyushu Institute of Technology.
#  Copyright (C) 2015- Shimane IT Open-Innovation Center.
#
#  This file is distributed under BSD 3-Clause License.
#

module Enumerable
  #
  # collect, map
  #
  def collect
    ret = []
    self.each {|item|
      ret << yield( item )
    }
    return ret
  end
  alias map collect

end
