
class ConstTest < Picotest::Test

  class A
    CONST = 1
    class B
      def should_raise
        puts B::CONST
      end
    end
  end

  description 'Should raise NameError'
  def test_name_error
    assert_equal 1, A::CONST
    b = A::B.new
    assert_raise(NameError) do
      b.should_raise
    end
  end

end

