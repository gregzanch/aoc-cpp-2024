#pragma once

#include <memory>
#include "day01/day01.h"

inline std::vector<std::unique_ptr<Solution>> get_days() {
  std::vector<std::unique_ptr<Solution>> v;
  v.emplace_back(std::make_unique<Day01>());
  return v;
};