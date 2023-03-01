#
# Integer, mrubyc class library
#
#  Copyright (C) 2015-2021 Kyushu Institute of Technology.
#  Copyright (C) 2015-2021 Shimane IT Open-Innovation Center.
#
#  This file is distributed under BSD 3-Clause License.
#

class Integer

  # times
  def times(&block)
    0.upto(self - 1, &block)
    return self
  end

  # upto
  def upto(max, &block)
    i = self
    while i <= max
      block.call i
      i += 1
    end
    return self
  end

end
