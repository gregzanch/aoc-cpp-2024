#pragma once

#include "../solution.h"

class Day01 : public Solution {
 public:
  DayResult solve() override {
    // parse the lines
    const auto path = std::filesystem::path("src/day01/input.txt");
    const auto read_result = read_input_lines(path);
    if (!read_result.has_value()) {
      return std::unexpected(read_result.error());
    }
    input = read_result.value();

    return std::tuple(part1(), part2());
  }

 private:
  PartResult part1() { return std::to_string(0); }
  PartResult part2() { return std::to_string(0); }

  std::vector<std::string> input;
};