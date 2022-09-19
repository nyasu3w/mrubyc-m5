# frozen_string_literal: true

class StringLjustRjustTest < MrubycTestCase
  description "ljust"
  def ljust_case
    assert_equal "foo    ", "foo".ljust(7)
    assert_equal "foo", "foo".ljust(3)
    assert_equal "foo****", "foo".ljust(7, "*")
    assert_equal "foo", "foo".ljust(1, "*")
    assert_equal "foo", "foo".ljust(-1, "*")
  end

  description "ljust object_id"
  def ljust_id_case
    obj = "foo"
    assert_not_equal obj.object_id, obj.ljust(0).object_id
  end

  description "ljust with multiple_characters of padding"
  def ljust_multiple_chars_padding_case
    assert_equal "fooabca", "foo".ljust(7, "abc")
    assert_equal "foo", "foo".ljust(3, "abc")
    assert_equal "abcabca", "".ljust(7, "abc")
  end

  description "ljust no Integer"
  def ljust_no_Integer_case
    assert_raise(TypeError.new("no implicit conversion into Integer")) do
      "foo".ljust("7")
    end
  end

  description "ljust zero width padding"
  def ljust_zero_width_padding_case
    assert_raise(ArgumentError.new("zero width padding")) do
      "foo".ljust(7, "")
    end
  end

  description "rjust"
  def rjust_case
    assert_equal "    bar", "bar".rjust(7)
    assert_equal "bar", "bar".rjust(3)
    assert_equal "****bar", "bar".rjust(7, "*")
    assert_equal "bar", "bar".rjust(1, "*")
    assert_equal "bar", "bar".ljust(-1, "*")
  end

  description "rjust object_id"
  def rjust_id_case
    obj = "bar"
    assert_not_equal obj.object_id, obj.rjust(0).object_id
  end

  description "rjust with multiple_characters of padding"
  def rjust_multiple_chars_padding_case
    assert_equal "abcabar", "bar".rjust(7, "abc")
    assert_equal "bar", "bar".rjust(3, "abc")
    assert_equal "abcabca", "".rjust(7, "abc")
  end

  description "rjust no Integer"
  def rjust_no_Integer_case
    assert_raise(TypeError.new("no implicit conversion into Integer")) do
      "bar".rjust("7")
    end
  end

  description "rjust zero width padding"
  def rjust_zero_width_padding_case
    assert_raise(ArgumentError.new("zero width padding")) do
      "bar".rjust(7, "")
    end
  end
end

