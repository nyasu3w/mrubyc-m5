# frozen_string_literal: true

class ConstTest < MrubycTestCase

  class A
    CONST = 1
    class B
      def should_raise
        puts B::CONST
      end
    end
  end

  description 'Should raise NameError'
  def name_error_case
    assert_equal 1, A::CONST
    b = A::B.new
    assert_raise(NameError) do
      b.should_raise
    end
  end

end

