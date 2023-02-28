# frozen_string_literal: true

class MyKargTest < MrubycTestCase
  def setup
    @obj = MyKarg.new
  end

  description 'karg with initial value'
  def karg_wiht_initial_value
    assert_equal 1, @obj.with_initial_value
    assert_equal 2, @obj.with_initial_value(k: 2)
  end

  description 'karg without initial value'
  def karg_without_initial_value
    assert_equal true, @obj.without_initial_value(k: true)
    assert_raise(ArgumentError.new "missing keywords: k") do
      @obj.without_initial_value
    end
  end

  description 'karg combined with m, opt, and rest'
  def karg_combined_with_m_opt_rest
    assert_equal 13, @obj.combined_with_m_opt_rest("dummy", "dummy", k3: 1, k2: 2, k1: 10)
  end

  description 'karg combined with dict'
  def karg_combined_with_dict
    assert_equal 10, @obj.combined_with_dict(k2: 2, k1: 1, k3: 3, k4: 4)
  end

  description 'karg in block argument'
  def karg_in_block_argument
    p = Proc.new do |a:, b: 11|
      a + b
    end
    assert_equal 25, p.call(b: 12, a: 13)
  end

  description 'yield case'
  def yield_case
    assert_equal({:a => 0}, @obj.yield_case(opt: {a: 0}) { |opt| opt })
  end

end

