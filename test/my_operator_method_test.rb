# frozen_string_literal: true

class MyOperatorMethodTest < MrubycTestCase
  def setup
    @obj = MyOperatorMethod.new
  end

  description 'defining operator methods'
  def defining_operator_methods
    assert_equal "== eq", (@obj == "eq")
    assert_equal "< lt",  (@obj < "lt")
    assert_equal "<= le", (@obj <= "le")
    assert_equal "> gt",  (@obj > "gt")
    assert_equal ">= ge", (@obj >= "ge")
  end
end

