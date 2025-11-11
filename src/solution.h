#pragma once

#include "util.h"
#include <tuple>

// Result from part 1 or 2 from a day
using PartResult = std::expected<std::string, std::string>;
// Result from the whole day
using DayResult = std::expected<std::tuple<PartResult, PartResult>, std::string>;

class Solution {
public:
  virtual DayResult solve() = 0;
  virtual ~Solution() = default;
};