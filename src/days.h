#pragma once

#include <memory>
#include "day01/day01.h"
#include "day02/day02.h"
#include "day03/day03.h"
#include "day04/day04.h"
#include "day05/day05.h"
#include "day06/day06.h"

inline std::vector<std::unique_ptr<Solution>> get_days() {
  std::vector<std::unique_ptr<Solution>> v;
  v.emplace_back(std::make_unique<Day01>());
  v.emplace_back(std::make_unique<Day02>());
  v.emplace_back(std::make_unique<Day03>());
  v.emplace_back(std::make_unique<Day04>());
  v.emplace_back(std::make_unique<Day05>());
  v.emplace_back(std::make_unique<Day06>());
  return v;
};