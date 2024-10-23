class MyMethodMissing
  def method_missing(name)
    puts "method_missing: #{name}"
  end
end

