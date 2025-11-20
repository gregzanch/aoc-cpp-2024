#pragma once

#include <algorithm>
#include <climits>
#include <cstring>
#include <iterator>
#include <print>
#include <ranges>
#include <set>
#include <unordered_set>
#include "../matrix.h"
#include "../solution.h"
#include "../util.h"
#include "../vec.h"

class Day06 : public Solution {
 public:
  DayResult solve() override {
    // parse the lines
    const auto path = std::filesystem::path("src/day06/input.txt");
    const auto read_result = read_input_lines(path);
    if (!read_result.has_value()) {
      return std::unexpected(read_result.error());
    }
    const auto lines = std::move(read_result.value());

    auto [poles, guard, bounds] = parse_locations(lines);

    this->poles = std::move(poles);
    this->guard = std::move(guard);
    this->bounds = std::move(bounds);

    return std::tuple(part1(), part2());
  }

 private:
  enum Direction : char {
    UP = 'u',
    DOWN = 'd',
    LEFT = 'l',
    RIGHT = 'r',
  };

  std::tuple<std::unordered_set<vec2<int>>, vec2<int>, vec2<int>> parse_locations(
      const std::vector<std::string>& lines) {
    std::unordered_set<vec2<int>> poles{};
    vec2<int> guard(0, 0);

    int x, y = 0;
    for (const auto& line : lines) {
      x = 0;
      for (const auto& c : line) {
        switch (c) {
          case '^':
            guard.set(x, y);
            break;
          case '#':
            poles.emplace(vec2<int>(x, y));
            break;
          default:
            break;
        }
        x++;
      }
      y++;
    }

    return std::tuple(std::move(poles), std::move(guard), vec2<int>(x, y));
  }

  bool find_closest_and_set() {
    switch (direction) {
      // when direction is up, find closest in - direction with const x;
      case Direction::UP: {
        auto min = INT_MAX;
        for (const auto& pole : poles) {
          // either non const x or pole not in same direction
          if (pole[0] != guard[0] || pole[1] > guard[1]) {
            continue;
          }

          auto distance = guard[1] - pole[1];
          if (distance <= min) {
            min = distance;
          }
        }

        if (min == INT_MAX) {
          for (auto i = guard[1]; i >= 0; i--) {
            visited_locations.emplace(vec2<int>(guard[0], i));
          }
          return true;
        } else {
          int i;
          for (i = guard[1]; i > guard[1] - min; i--) {
            vec2<int> l(guard[0], i);
            // std::println("{}", l);
            visited_locations.emplace(std::move(l));
          }
          guard.set(guard[0], i + 1);
          direction = Direction::RIGHT;
        }
      } break;
      // when direction is down find closest in + direction with const x;
      case Direction::DOWN: {
        auto min = INT_MAX;
        for (const auto& pole : poles) {
          // either non const x or pole not in same direction
          if (pole[0] != guard[0] || pole[1] < guard[1]) {
            continue;
          }

          auto distance = pole[1] - guard[1];

          if (distance <= min) {
            min = distance;
          }
        }

        if (min == INT_MAX) {
          for (int i = guard[1]; i < bounds[1]; i++) {
            visited_locations.emplace(vec2<int>(guard[0], i));
          }
          return true;
        } else {
          int i;
          for (i = guard[1]; i < guard[1] + min; i++) {
            vec2<int> l(guard[0], i);
            // std::println("{}", l);
            visited_locations.emplace(std::move(l));
          }
          guard.set(guard[0], i - 1);
          direction = Direction::LEFT;
        }

      } break;
      // when direction is left find closest in - direction with const y;
      case Direction::LEFT: {
        auto min = INT_MAX;
        for (const auto& pole : poles) {
          if (pole[1] != guard[1] || pole[0] > guard[0]) {
            continue;
          }
          auto distance = guard[0] - pole[0];
          if (distance <= min) {
            min = distance;
          }
        }
        if (min == INT_MAX) {
          for (int i = guard[0]; i >= 0; i--) {
            visited_locations.emplace(vec2<int>(i, guard[1]));
          }
          return true;
        } else {
          int i;
          for (i = guard[0]; i > guard[0] - min; i--) {
            vec2<int> l(i, guard[1]);
            // std::println("{}", l);
            visited_locations.emplace(std::move(l));
          }
          guard.set(i + 1, guard[1]);
          direction = Direction::UP;
        }
      } break;
      // when direction is right find closest in + direction with const y;
      case Direction::RIGHT: {
        auto min = INT_MAX;
        for (const auto& pole : poles) {
          if (pole[1] != guard[1] || pole[0] < guard[0]) {
            continue;
          }
          auto distance = pole[0] - guard[0];
          if (distance <= min) {
            min = distance;
          }
        }
        if (min == INT_MAX) {
          for (int i = guard[0]; i < bounds[0]; i++) {
            visited_locations.emplace(vec2<int>(i, guard[1]));
          }
          return true;
        } else {
          int i;
          for (i = guard[0]; i < guard[0] + min; i++) {
            vec2<int> l(i, guard[1]);
            // std::println("{}", l);
            visited_locations.emplace(std::move(l));
          }
          guard.set(i - 1, guard[1]);
          direction = Direction::DOWN;
        }
      } break;
      default:
        break;
    }
    return false;
  }

  PartResult part1() {
    // std::println("direc: {}", (char)direction);
    // std::println("guard: {}", guard);
    while (!find_closest_and_set()) {
      // std::println("direc: {}", (char)direction);
      // std::println("guard: {}", guard);
    }
    for (const auto& loc : visited_locations) {
      // std::println("{}", loc);
    }

    Matrix<std::string> m{};
    std::vector<std::vector<std::string>> d;
    for (int i = 0; i < bounds[1]; i++) {
      std::vector<std::string> r{};
      for (int j = 0; j < bounds[0]; j++) {
        if (visited_locations.contains(vec2<int>(j, i))) {
          r.push_back("█");
        } else if (poles.contains(vec2<int>(j, i))) {
          r.push_back("#");
        } else {
          r.push_back(" ");
        }
      }
      d.push_back(std::move(r));
    }
    m.set_data(std::move(d));

    // std::println("{}", m);

    return std::to_string(visited_locations.size());
  }
  PartResult part2() { return std::to_string(0); }
  Direction direction = Direction::UP;
  vec2<int> bounds;
  vec2<int> guard;
  std::unordered_set<vec2<int>> poles;
  std::unordered_set<vec2<int>> visited_locations;
};