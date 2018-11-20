
#pragma once

#include <vector>

class ResultChart {
public:
  ResultChart() = delete;

  static void chowResult(const std::vector<std::pair<int, double>>&, const std::vector<std::pair<int, double>>&);
};

