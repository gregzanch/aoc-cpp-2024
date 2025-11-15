#pragma once

#include <algorithm>
#include <iterator>
#include <map>
#include <ranges>
#include <set>
#include "../solution.h"

class Day05 : public Solution {
 public:
  DayResult solve() override {
    // parse the lines
    const auto path = std::filesystem::path("src/day05/input.txt");
    const auto read_result = read_input_lines(path);
    if (!read_result.has_value()) {
      return std::unexpected(read_result.error());
    }

    const auto input = std::move(read_result.value());

    auto it = std::ranges::find_if(input, is_empty_string);
    if (it == input.end()) {
      return std::unexpected("Couldn't split input into two sections");
    }
    int index = std::ranges::distance(input.begin(), it);

    for (int i = 0; i < input.size(); i++) {
      // skip over seperator
      if (i == index) {
        continue;
      }

      // parse rules
      if (i < index) {
        int left = std::stoi(input[i].substr(0, 2));
        int right = std::stoi(input[i].substr(3, 2));
        rules.emplace_back(std::tuple(left, right));
        continue;
      }

      // parse update
      if (i > index) {
        const auto update = split_by(input[i], ",", [](const auto s) { return std::stoi(s); });
        updates.push_back(std::move(update));
      }
    }

    for (const auto& rule : rules) {
      const auto [left, right] = rule;
      if (!key_is_less_than.contains(left)) {
        const std::set<int> s;
        key_is_less_than.emplace(left, s);
      }
      key_is_less_than.at(left).emplace(right);
    }

    return std::tuple(part1(), part2());
  }

 private:
  bool is_less_than(int a, int b) {
    return key_is_less_than.contains(a) && key_is_less_than.at(a).contains(b);
  }

  bool update_follows_rule(const std::vector<int>& update, const std::tuple<int, int>& rule) {
    const auto [first, second] = rule;
    int first_index = -1;
    int second_index = -1;
    auto first_it = std::ranges::find(update, first);
    if (first_it != update.end()) {
      first_index = std::distance(update.begin(), first_it);
    }
    auto second_it = std::ranges::find(update, second);
    if (second_it != update.end()) {
      second_index = std::distance(update.begin(), second_it);
    }

    if (first_index == -1 || second_index == -1) {
      return true;
    }

    if (first_index < second_index) {
      return true;
    }

    return false;
  }

  int get_center(const std::vector<int>& update) { return update.at(update.size() >> 1); }

  PartResult part1() {
    int sum = 0;
    for (const auto& update : updates) {
      bool follows_rules = true;
      for (const auto& rule : rules) {
        follows_rules = follows_rules && update_follows_rule(update, rule);
      }
      if (follows_rules) {
        const auto center = get_center(update);
        sum += center;
      }
    }
    return std::to_string(sum);
  }
  PartResult part2() {
    int sum = 0;
    // we want to sum the centers of the rearranged ones
    for (auto& update : updates) {
      auto followed_rules = true;
      for (const auto& rule : rules) {
        if (!update_follows_rule(update, rule)) {
          followed_rules = false;
        }
      }
      if (!followed_rules) {
        std::sort(update.begin(), update.end(), [this](int a, int b) {
          // a < b
          if (this->is_less_than(a, b)) {
            return false;
          }

          // a > b
          if (this->is_less_than(b, a)) {
            return true;
          }

          // a == b
          return false;
        });
        const auto center = get_center(update);
        sum += center;
      }
    }
    return std::to_string(sum);
  }

  std::map<int, std::set<int>> key_is_less_than;
  std::vector<std::tuple<int, int>> rules;
  std::vector<std::vector<int>> updates;
};