#pragma once

#include <cstring>
#include <string>
#include "../matrix.h"
#include "../solution.h"

class Day04 : public Solution {
 public:
  DayResult solve() override {
    // parse the lines
    const auto path = std::filesystem::path("src/day04/input.txt");
    const auto read_result = read_input_lines(path);
    if (!read_result.has_value()) {
      return std::unexpected(read_result.error());
    }

    std::vector<std::vector<char>> input;
    for (const auto& line : read_result.value()) {
      std::vector<char> row;
      for(int i = 0; i<line.size(); i++) {
        row.emplace_back(line.at(i));
      }
      input.emplace_back(std::move(row));
    }

    word_search.set_data(std::move(input));

    return std::tuple(part1(), part2());
  }

 private:
  bool search(size_t x, size_t y, int x_direction, int y_direction) {
    // make sure we can do the search
    if (!within(x + 3 * x_direction, 0uz, word_search.width(), LEFT_INCLUSIVE) ||
        !within(y + 3 * y_direction, 0uz, word_search.height(), LEFT_INCLUSIVE)) {
      return false;
    }

    std::vector<std::tuple<size_t, size_t>> indices{
      std::tuple(x + 0 * x_direction, y + 0 * y_direction),
      std::tuple(x + 1 * x_direction, y + 1 * y_direction),
      std::tuple(x + 2 * x_direction, y + 2 * y_direction),
      std::tuple(x + 3 * x_direction, y + 3 * y_direction),
    };

    std::string formed_string = "";
    for (const auto& location : indices) {
      const auto [x_index, y_index] = location;
      formed_string.push_back(word_search.get(x_index, y_index).value());
    }

    if(formed_string == "XMAS") {
      return true;
    }

    return false;

  }
  PartResult part1() {
    const std::array<std::tuple<int, int>, 8> directions {
      std::tuple(0, 1), // up
      std::tuple(1, 0), // right
      std::tuple(0, -1), // down
      std::tuple(-1, 0), // left
      std::tuple(1, 1), // up right
      std::tuple(1, -1), // down right
      std::tuple(-1, -1), // down left
      std::tuple(-1, 1), // up left
    };
    int total_count = 0;
    for(int x = 0; x<word_search.width(); x++){
      for(int y = 0; y<word_search.height(); y++) {
        int match_count = 0;
        for(const auto& direction : directions) {
          const auto [x_dir, y_dir] = direction;
          if(search(x, y, x_dir, y_dir)) {
            match_count++;
          }
        }
        total_count+=match_count;
      }
    }
    return std::to_string(total_count);
  }
  PartResult part2() { return std::to_string(0); }

  Matrix<char> word_search;
};