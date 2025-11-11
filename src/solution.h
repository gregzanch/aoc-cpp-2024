#pragma once

#include "util.h"
#include <tuple>

// Result from a single day
using DayResult = std::expected<std::string, std::string>;

class Solution {
public:
  virtual std::tuple<DayResult, DayResult> solve() = 0;
  virtual ~Solution() = default;
};