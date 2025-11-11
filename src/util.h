#pragma once

#include <expected>
#include <filesystem>
#include <format>
#include <fstream>
#include <optional>
#include <regex>
#include <sstream>
#include <string>
#include <type_traits>
#include <unordered_map>
#include <vector>

template <typename NumericType>
concept Numeric = std::is_arithmetic<NumericType>::value;

template <typename K>
concept Iterable = std::ranges::range<K>;

using Lines = std::vector<std::string>;
using ReadLinesResult = std::expected<Lines, std::string>;
auto read_input_lines(const std::filesystem::path& path) -> ReadLinesResult {
  if (!std::filesystem::exists(path)) {
    return std::unexpected(std::format("{} does not exist!", path.string()));
  }
  std::ifstream input_file(path);
  if (!input_file.is_open()) {
    return std::unexpected(
        std::format("{} is currently open. Close it to continue", path.string()));
  }

  std::vector<std::string> result;

  std::string line;
  while (std::getline(input_file, line)) {
    result.emplace_back(line);
  }

  return result;
}

using ReadFileResult = std::expected<std::string, std::string>;
auto read_input_file(const std::filesystem::path& path) -> ReadFileResult {
  if (!std::filesystem::exists(path)) {
    return std::unexpected(std::format("{} does not exist!", path.string()));
  }
  std::ifstream input_file(path);
  if (!input_file.is_open()) {
    return std::unexpected(
        std::format("{} is currently open. Close it to continue", path.string()));
  }

  std::ostringstream buffer;
  buffer << input_file.rdbuf();
  return buffer.str();
}

constexpr uint8_t EXCLUSIVE = 0b00;
constexpr uint8_t RIGHT_INCLUSIVE = 0b01;
constexpr uint8_t LEFT_INCLUSIVE = 0b10;
constexpr uint8_t BOTH_INCLUSIVE = 0b11;

template <Numeric T, Numeric A, Numeric B>
inline bool within(T value, A lower, B upper, uint8_t inclusivity = EXCLUSIVE) {
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

template <std::ranges::range K>
auto bag(const K& input) {
  using T = std::ranges::range_value_t<K>;
  std::unordered_map<T, int> result;
  for (auto&& elem : input) {
    ++result[elem];
  }
  return result;
}
template <typename Parser>
concept StringParser = requires(Parser p, std::string s) {
  { p(s) };
};

template <StringParser Parser>
auto split_by(
    const std::string& input,
    const std::string& delimiter,
    Parser parser = [](std::string s) { return s; }) {
  using T = std::invoke_result_t<Parser, std::string>;
  std::vector<T> result;

  size_t start = 0;
  size_t end;

  while ((end = input.find(delimiter, start)) != std::string::npos) {
    std::string token = input.substr(start, end - start);
    if (!token.empty()) {
      result.emplace_back(parser(token));
    }
    start = end + delimiter.length();
  }

  std::string last = input.substr(start);
  if (!last.empty()) {
    result.emplace_back(parser(last));
  }

  return result;
}

using MatchResult = std::optional<std::vector<std::vector<std::string>>>;
MatchResult basic_match(const std::string& input, const std::regex& pattern) {
  std::vector<std::vector<std::string>> result;

  auto begin = std::sregex_iterator(input.begin(), input.end(), pattern);
  auto end = std::sregex_iterator();
  auto count = 0;
  for (auto it = begin; it != end; ++it) {
    const std::smatch& match = *it;
    std::vector<std::string> g{};
    for (size_t groupIndex = 0; groupIndex < match.size(); ++groupIndex) {
      g.emplace_back(match[groupIndex].str());
      count++;
    }
    result.emplace_back(std::move(g));
  }
  if (count) {
    return result;
  }
  return {};
}

template <typename T>
std::vector<T> flatten(const std::vector<std::vector<T>>& iterable) {
  std::vector<T> result;
  for (const auto& row : iterable) {
    for (const auto& item : row) {
      result.emplace_back(item);
    }
  }
  return result;
}

inline bool is_empty_string(const std::string& str) {
  return str == "";
}