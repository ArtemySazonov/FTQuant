#include "LocalVolatility.hpp"

LocalVolatility::LocalVolatility() : r(0.) {
    sigma = [](double a, double b)
    { return 0.; };
}

std::vector<std::vector<double>> LocalVolatility::generate_paths(int n_paths) {
  return std::vector<std::vector<double>>();
}
