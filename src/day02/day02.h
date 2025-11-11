#pragma once

#include "../solution.h"

class Day02 : public Solution {
 public:
  DayResult solve() override {
    // parse the lines
    const auto path = std::filesystem::path("src/day02/input.txt");
    const auto read_result = read_input_lines(path);
    if (!read_result.has_value()) {
      return std::unexpected(read_result.error());
    }
    input = read_result.value();

    for (const auto& line : input) {
      const auto report =
          split_by(line, " ", [](std::string item) { return std::atoi(item.c_str()); });
      reports.emplace_back(report);
    }

    return std::tuple(part1(), part2());
  }

 private:
  bool is_safe_report(const std::vector<int>& report) {
    bool should_increase = report.at(0) < report.back();
    bool first_passed = false;
    int prev = 0;
    for (const auto& current : report) {
      if (!first_passed) {
        first_passed = true;
        prev = current;
        continue;
      }
      if (!within(abs(current - prev), 1, 3, BOTH_INCLUSIVE)) {
        return false;
      }
      bool is_increasing = current > prev;
      if (should_increase ^ is_increasing) {
        return false;
      }
      prev = current;
    }
    return true;
  }

  PartResult part1() {
    int count = 0;
    for (const auto& report : reports) {
      if (is_safe_report(report)) {
        count++;
      }
    }
    return std::to_string(count);
  }
  PartResult part2() {
    int count = 0;
    for (const auto& report : reports) {
      if (is_safe_report(report)) {
        count++;
        continue;
      }
      for (auto i = 0; i < report.size(); i++) {
        std::vector<int> copied_report(report);
        copied_report.erase(copied_report.begin() + i);
        if (is_safe_report(copied_report)) {
          count++;
          break;
        }
      }
    }
    return std::to_string(count);
  }

  std::vector<std::vector<int>> reports;
  std::vector<std::string> input;
};