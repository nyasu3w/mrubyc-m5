# frozen_string_literal: true

class MyMethodMissingTest < MrubycTestCase
  description 'method_missing'
  def method_missing_case
    assert_equal("method_missing: foo", MyMethodMissing.new.foo)
  end
end

