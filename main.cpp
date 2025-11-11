#include <cstdlib>
#include <print>
#include "src/days.h"


void help(const char** argv) {
  const auto program = argv[0];
  std::println("Usage:");
  std::println("{} <day-number>", program);
}

int main(int argc, const char** argv) {
  if(argc != 2) {
    help(argv);
    return 1;
  }

  auto days = get_days();

  size_t day_number = atoi(argv[1]) - 1;
  if(!within(day_number, 0uz, days.size(), LEFT_INCLUSIVE)) {
    std::println("{} out of bounds", day_number);
    return 1;
  }

  const auto day_result = days.at(day_number)->solve();
  if(!day_result.has_value()) {
    std::println("Day {} had error: {}", day_number, day_result.error());
    return 1;
  }

  const auto [part1, part2] = day_result.value();

  if(!part1.has_value()) {
    std::println("Day {} part 1 had error: {}", day_number, part1.error());
    return 1;
  }

  if(!part2.has_value()) {
    std::println("Day {} part 2 had error: {}", day_number, part2.error());
    return 1;
  }

  std::println("Part 1: {}", part1.value());
  std::println("Part 2: {}", part2.value());

  return 0;
}