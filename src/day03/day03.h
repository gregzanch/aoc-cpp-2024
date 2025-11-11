#pragma once

#include <regex>
#include "../solution.h"

class Day03 : public Solution {
 public:
  DayResult solve() override {
    // parse the lines
    const auto path = std::filesystem::path("src/day03/input.txt");
    const auto read_result = read_input_file(path);
    if (!read_result.has_value()) {
      return std::unexpected(read_result.error());
    }
    input = std::move(read_result.value());

    return std::tuple(part1(), part2());
  }

 private:
  PartResult part1() {
    std::regex regex(R"(mul\(\d{1,3},\d{1,3}\))");

    auto matches = basic_match(input, regex);
    if (!matches.has_value()) {
      return std::unexpected("couldn't find any mul(a,b) statements");
    }

    std::vector<std::string> mul_statements = flatten(matches.value());

    int sum = 0;
    std::regex digit_regex(R"(\d{1,3})");
    for (const auto& statement : mul_statements) {
      auto matches = basic_match(statement, digit_regex);
      if (!matches.has_value()) {
        return std::unexpected(std::format("failed to parse mul statement {}", statement));
      }
      auto args = flatten(matches.value());
      const int a = std::atoi(args.at(0).c_str());
      const int b = std::atoi(args.at(1).c_str());

      sum += a * b;
    }
    return std::to_string(sum);
  }
  PartResult part2() {
    std::regex regex(R"(mul\(\d{1,3},\d{1,3}\)|don't\(\)|do\(\))");

    auto matches = basic_match(input, regex);
    if (!matches.has_value()) {
      return std::unexpected("couldn't find any mul(a,b) statements");
    }

    std::vector<std::string> mul_statements = flatten(matches.value());

    int sum = 0;
    std::regex digit_regex(R"(\d{1,3})");
    bool enabled = true;
    for (const auto& statement : mul_statements) {
      if (statement.starts_with("don't()")) {
        enabled = false;
        continue;
      } else if (statement.starts_with("do()")) {
        enabled = true;
        continue;
      }
      if (enabled) {
        auto matches = basic_match(statement, digit_regex);
        if (!matches.has_value()) {
          return std::unexpected(std::format("failed to parse mul statement {}", statement));
        }
        auto args = flatten(matches.value());
        const int a = std::atoi(args.at(0).c_str());
        const int b = std::atoi(args.at(1).c_str());

        sum += a * b;
      }
    }
    return std::to_string(sum);
  }

  std::string input;
};