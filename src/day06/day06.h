#pragma once

#include <algorithm>
#include <cstring>
#include <iterator>
#include <set>
#include <ranges>
#include "../solution.h"
#include "../matrix.h"
#include "../vec.h"
#include "../util.h"

class Day06 : public Solution {
 public:
  DayResult solve() override {
    // parse the lines
    const auto path = std::filesystem::path("src/day06/example.txt");
    const auto read_result = read_input_lines(path);
    if (!read_result.has_value()) {
      return std::unexpected(read_result.error());
    }
    const auto lines = std::move(read_result.value());
    
    std::vector<std::vector<char>> m{};

    int y = 0;
    for(const auto& line : lines) {
      m.emplace_back(std::vector<char>(line.begin(), line.end()));
      const auto it = std::ranges::find_if(m.back(), [](char c){ return c == '^'; });
      if(it != m.back().end()) {
        guard_location.data[0] = std::distance(m.back().begin(), it);
        guard_location.data[1] = y;

      }
      y++;
    }

    map.set_data(std::move(m));

    return std::tuple(part1(), part2());
  }

 private:
  PartResult part1() {
    std::println("{}", guard_location);
    return std::to_string(0);
  }
  PartResult part2() { return std::to_string(0); }
  
  vec2<int> guard_location;
  std::set<vec2<int>> visited_locations;
  Matrix<char> map;
  std::vector<std::string> input;
};