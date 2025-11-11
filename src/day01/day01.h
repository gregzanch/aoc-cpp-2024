#pragma once

#include "../solution.h"

class Day01 : public Solution {
public:
  std::tuple<DayResult, DayResult> solve() override {
    return std::tuple(part1(), part2());
  }

private:
  DayResult part1() { return "part1"; }
  DayResult part2() { return "part2"; }
};