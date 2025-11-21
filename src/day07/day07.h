#pragma once

#include <sys/types.h>
#include <print>
#include <string>
#include <unordered_set>
#include "../solution.h"

using BigInt = unsigned long long;

struct Equation {
  BigInt value;
  std::vector<BigInt> arguments;
  Equation(std::string line) {
    auto split_line = split_by(line, ":", [](const std::string& s){
      return s;
    });

    this->value = std::stoull(split_line[0]);
    this->arguments = split_by(split_line[1], " ", [](const std::string& s) {
      return std::stoull(s);
    });
  };
  void print() {
    std::println("{}", value);
    std::println("{}", arguments);
  }
};

class Day07 : public Solution {
 public:
  DayResult solve() override {
    // parse the lines
    const auto path = std::filesystem::path("src/day07/input.txt");
    const auto read_result = read_input_lines(path);
    if (!read_result.has_value()) {
      return std::unexpected(read_result.error());
    }

    for(const auto& line : read_result.value()) {
      equations.emplace_back(Equation(line));
      equations.back().print();
    }

    return std::tuple(part1(), part2());
  }

 private:

  bool achievable(Equation eq) {
    std::unordered_set<BigInt> current, next;
    current.insert(eq.arguments[0]);

    for(auto i = 1; i<eq.arguments.size(); i++){
      BigInt x = eq.arguments[i];
      next.clear();
      next.reserve(current.size() * 2);
      for(BigInt v : current) {
        next.insert(v + x);
        next.insert(v * x);
      }
      current = next;

      if(current.contains(eq.value)) {
        return true;
      }
    }

    return current.contains(eq.value);
  }

  PartResult part1() {
    BigInt sum = 0;
    for(const auto& equation : equations) {
      if(achievable(equation)) {
        sum += equation.value;
      }
    }
    return std::to_string(sum);
  }
  PartResult part2() { return std::to_string(0); }
  std::vector<Equation> equations;
};