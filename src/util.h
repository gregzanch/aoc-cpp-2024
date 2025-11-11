#pragma once

#include <type_traits>
#include <filesystem>
#include <format>
#include <fstream>
#include <optional>
#include <expected>
#include <string>
#include <vector>

using Lines = std::vector<std::string>;
using ReadLinesResult = std::expected<Lines, std::string>;

inline auto read_input_lines(const std::filesystem::path &path) -> ReadLinesResult {
  if(!std::filesystem::exists(path)) {
    return std::unexpected(std::format("{} does not exist!", path.string()));
  }
  std::ifstream input_file(path);
  if(!input_file.is_open()) {
    return std::unexpected(std::format("{} is currently open. Close it to continue", path.string()));
  }

  std::vector<std::string> result;

  std::string line;
  while(std::getline(input_file, line)) {
    result.emplace_back(line);
  }

  return result;  
}

template<typename NumericType>
concept Numeric = std::is_arithmetic<NumericType>::value;

constexpr uint8_t EXCLUSIVE = 0b00;
constexpr uint8_t RIGHT_INCLUSIVE = 0b01;
constexpr uint8_t LEFT_INCLUSIVE = 0b10;
constexpr uint8_t BOTH_INCLUSIVE = 0b11;

template<Numeric T>
bool within(T value, T lower, T upper, uint8_t inclusivity = EXCLUSIVE) {
  // inclusivity bit pattern:
  // 0b00 -> (lower, upper)
  // 0b01 -> (lower, upper]
  // 0b10 -> [lower, upper)
  // 0b11 -> [lower, upper]

  bool lower_inclusive = inclusivity & 0b10;
  bool upper_inclusive = inclusivity & 0b01;

  bool lower_ok = lower_inclusive ? value >= lower : value > lower;
  bool upper_ok = upper_inclusive ? value <= upper : value < upper;

  return lower_ok && upper_ok;
}
