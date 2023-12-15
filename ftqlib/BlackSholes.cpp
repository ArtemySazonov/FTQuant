#include "BlackSholes.hpp"
#include <random>
#include <iostream>

std::vector<std::vector<double>> BlackScholes::generate_paths(
    int n_paths, int steps, double T, double spot, bool antithetic) {
  std::random_device rd{};
  std::mt19937 gen(rd());

  std::vector<std::vector<double>> paths;
  std::normal_distribution<double> Normal(0, 1);
  double dt = T / steps;
  double sigma = this->sigma;
  double sqdt = sqrt(dt) * sigma;
  double r_simga_dt = (this->r - 0.5 * sigma * sigma) * dt;

  if (antithetic) {
    for (int i = 0; i < n_paths; ++i) {

      std::vector<double> path1;
      std::vector<double> path2;
      path1.push_back(0);
      path2.push_back(0);

      for (int j = 1; j <= steps; ++j) {
        double sigma_dw = sqdt * Normal(gen);
        path1.push_back(path1[j - 1] + r_simga_dt + sigma_dw);
        path1[j - 1] = spot * exp(path1[j - 1]);

        path2.push_back(path2[j - 1] + r_simga_dt - sigma_dw);
        path2[j - 1] = spot * exp(path2[j - 1]);
      }
      path1[path1.size() - 1] = spot * exp(path1[path1.size() - 1]);
      path2[path2.size() - 1] = spot * exp(path2[path2.size() - 1]);

      // std::cout << "path: " << path1.size() << std::endl;

      paths.push_back(path1);
      paths.push_back(path2);
    }
  } else {
    for (int i = 0; i < n_paths; ++i) {

      std::vector<double> path1;
      path1.push_back(0);

      for (int j = 1; j <= steps; ++j) {
        double sigma_dw = sqdt * Normal(gen);
        path1.push_back(path1[j - 1] + r_simga_dt + sigma_dw);
        path1[j - 1] = spot * exp(path1[j - 1]);
      }
      path1[path1.size() - 1] = spot * exp(path1[path1.size() - 1]);

      paths.push_back(path1);
    }
  }
  return paths;
}

void BlackScholes::calibrate(std::vector<double>& Stock_prices) {
  double mean = 0;
  double std = 0.;
  int N = Stock_prices.size();
  for (auto stock_price : Stock_prices)
    mean += (stock_price / N);
  for (auto stock_price : Stock_prices)
    std += (stock_price - mean) * (stock_price - mean) / (N - 1);
  this->r = mean;
  this->sigma = sqrt(std);
  return;
}