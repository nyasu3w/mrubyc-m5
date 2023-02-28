class MyKarg

  def with_initial_value(k: 1)
    k
  end

  def without_initial_value(k:)
    k
  end

  def combined_with_m_opt_rest(a, b, o1 = 1, o2 = 2, *c, k1:, k2: 3, k3:)
    k1 + k2 + k3
  end

  def combined_with_dict(k1: 0, k2:, **dict)
    k1 + k2 + dict[:k3] + dict[:k4]
  end

  def yield_case(opt: {})
    yield(opt)
  end

end
