#pragma once

#include "../solution.h"
#include <algorithm>
class Day01 : public Solution {
public:
  DayResult solve() override {
    // parse the lines
    const auto path = std::filesystem::path("src/day01/input.txt");
    const auto read_result = read_input_lines(path);
    if (!read_result.has_value()) {
      return std::unexpected(read_result.error());
    }
    input = std::move(read_result.value());

    // split into lists
    for (const auto &line : input) {
      const auto first = line.substr(0, line.find_first_of(" "));
      const auto second = line.substr(line.find_last_of(" "), line.size() - 1);
      left_list.emplace_back(std::stoi(first));
      right_list.emplace_back(std::stoi(second));
    }

    return std::tuple(part1(), part2());
  }

private:
  PartResult part1() {
    std::sort(left_list.begin(), left_list.end());
    std::sort(right_list.begin(), right_list.end());
    int sum = 0;
    for (int i = 0; i < left_list.size(); i++) {
      const auto difference = abs(left_list.at(i) - right_list.at(i));
      sum += difference;
    }
    return std::to_string(sum);
  }
  PartResult part2() { 
    int sum = 0;
    const auto right_bag = bag(right_list);
    for(const auto& item : left_list) {
      if(right_bag.contains(item)) {
        sum += item * right_bag.at(item);
      }
    }
    return std::to_string(sum);
  }

  std::vector<int> left_list;
  std::vector<int> right_list;
  std::vector<std::string> input;
};