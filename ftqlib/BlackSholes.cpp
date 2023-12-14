#include <algorithm>
#include <random>

#include <BlackSholes.hpp>

std::vector<std::vector<double>> BlackScholes::generate_paths(
    int n_paths, int steps, double T, double spot, bool antithetic) {
  std::random_device rd{};
  std::mt19937 gen(rd());

  std::vector<std::vector<double>> paths;
  std::normal_distribution<double> Normal(0, 1);
  double dt = T / steps;
  double sqdt = sqrt(dt) * sigma;
  double r_sigma_dt = (this->r - 0.5 * sigma * sigma) * dt;

  if (antithetic) {
    for (int i = 0; i < n_paths; ++i) {
      std::vector<double> path1;
      std::vector<double> path2;
      path1.push_back(0);
      path2.push_back(0);

      for (int j = 1; j <= steps; ++j) {
        double sigma_dw = sqdt * Normal(gen);
        path1.push_back(path1[j - 1] + r_sigma_dt + sigma_dw);
        path1[j - 1] = spot * exp(path1[j - 1]);

        path2.push_back(path2[j - 1] + r_sigma_dt - sigma_dw);
        path2[j - 1] = spot * exp(path2[j - 1]);
      }
      path1[path1.size() - 1] = spot * exp(path1[path1.size() - 1]);
      path2[path2.size() - 1] = spot * exp(path2[path2.size() - 1]);

      paths.push_back(path1);
      paths.push_back(path2);
    }
  } else {
    for (int i = 0; i < n_paths; ++i) {
      std::vector<double> path1;
      path1.push_back(0);

      for (int j = 1; j <= steps; ++j) {
        double sigma_dw = sqdt * Normal(gen);
        path1.push_back(path1[j - 1] + r_sigma_dt + sigma_dw);
        path1[j - 1] = spot * exp(path1[j - 1]);
      }
      path1[path1.size() - 1] = spot * exp(path1[path1.size() - 1]);

      paths.push_back(path1);
    }
  }
  return paths;
}

void BlackScholes::calibrate(std::vector<double>& stock_prices) {
  double mean = 0;
  double std = 0.;
  int N = stock_prices.size();
  for (auto stock_price : stock_prices)
    mean += (stock_price / N);
  for (auto stock_price : stock_prices)
    std += (stock_price - mean) * (stock_price - mean) / (N - 1);
  this->r = mean;
  this->sigma = sqrt(std);
}

std::vector<std::vector<double>> BlackScholes::pde_pricer(
    double r, double sigma, double T, std::function<double(double)> payoff,
    double S_max, double S_min, int M, int N)
{
  double h = (S_max - S_min) / N;               //s step
  double tau = T / M;                           //time step
  std::vector<std::vector<double>> u;           // create a return matrix
  u.resize(M + 1, std::vector<double>(N + 1));  //set the required dimensions
  //Step 1:initialize the zero time layer with the payoff function
  for (int j = 0; j < N + 1; j++) {
    u[0][j] = payoff(S_min + j * h);
  }
  //Step 2:calculate first time layer
  u[1][1] = u[0][1] +
            tau * (pow(sigma, 2) * pow(S_min + h, 2) *
                       (u[0][2] - 2 * u[0][1] + u[0][0]) / 4 / pow(h, 2) +
                   r * (S_min + h) * (u[0][2] - u[0][0]) / 2 / h - r * u[0][1]);
  u[1][N - 1] =
      u[0][N - 1] +
      tau * (pow(sigma, 2) * pow(S_min + (N - 1) * h, 2) *
                 (u[0][N] - 2 * u[0][N - 1] + u[0][N - 2]) / 4 / pow(h, 2) +
             r * (S_min + (N - 1) * h) * (u[0][N] - u[0][N - 2]) / 2 / h -
             r * u[0][N - 1]);
  std::vector<double> a;
  a.resize(N);
  std::vector<double> b;
  b.resize(N);
  std::vector<double> c;
  c.resize(N);
  std::vector<double> f;
  f.resize(N);
  c[1] = 1;
  b[1] = 0;
  f[1] = u[1][1];
  for (int j = 2; j <= N - 2; j++) {
    b[j] = a[j] = tau * pow(sigma, 2) * pow(S_min + j * h, 2) / 4 / pow(h, 2);
    c[j] = 1 + tau * pow(sigma, 2) * pow(S_min + j * h, 2) / 2 / pow(h, 2);
    f[j] = tau *
               (pow(sigma, 2) * pow(S_min + j * h, 2) / 4 / pow(h, 2) -
                r * (S_min + j * h) / 2 / h) *
               u[0][j - 1] -
           tau * (pow(sigma, 2) * pow(S_min + j * h, 2) / 2 / pow(h, 2) + r) *
               u[0][j] +
           u[0][j] +
           tau *
               (pow(sigma, 2) * pow(S_min + j * h, 2) / 4 / pow(h, 2) +
                r * (S_min + j * h) / 2 / h) *
               u[0][j + 1];
  }
  c[N - 1] = 1;
  a[N - 1] = 0;
  f[N - 1] = u[1][N - 1];
  thomas_algorithm(u[1], a, b, c, f);
  u[1][0] = 2 * u[1][1] - u[1][2];
  u[1][N] = 2 * u[1][N - 1] - u[1][N - 2];
  //Step 3:calculate remaining time layers
  for (int k = 2; k <= M; k++) {
    u[k][1] = u[k - 1][1] +
              tau * (r * (S_min + h) * (u[k - 1][2] - u[k - 1][0]) / 2 / h -
                     r * u[k - 1][1]);
    u[k][N - 1] =
        u[k - 1][N - 1] + tau * (r * (S_min + (N - 1) * h) *
                                     (u[k - 1][N] - u[k - 1][N - 2]) / 2 / h -
                                 r * u[k - 1][N - 1]);
    c[1] = 1;
    b[1] = 0;
    f[1] = u[k][1];
    for (int j = 2; j <= N - 2; j++) {
      b[j] = a[j] = tau * pow(sigma, 2) * pow(S_min + j * h, 2) / 4 / pow(h, 2);
      c[j] = 1 + tau * pow(sigma, 2) * pow(S_min + j * h, 2) / 2 / pow(h, 2);
      f[j] = tau *
                 (pow(sigma, 2) * pow(S_min + j * h, 2) / 4 / pow(h, 2) -
                  r * (S_min + j * h) / 2 / h) *
                 u[k - 1][j - 1] -
             tau * (pow(sigma, 2) * pow(S_min + j * h, 2) / 2 / pow(h, 2) + r) *
                 u[k - 1][j] +
             u[k - 1][j] +
             tau *
                 (pow(sigma, 2) * pow(S_min + j * h, 2) / 4 / pow(h, 2) +
                  r * (S_min + j * h) / 2 / h) *
                 u[k - 1][j + 1];
    }
    c[N - 1] = 1;
    a[N - 1] = 0;
    f[N - 1] = u[k][N - 1];
    thomas_algorithm(u[k], a, b, c, f);
    u[k][0] = 2 * u[k][1] - u[k][2];
    u[k][N] = 2 * u[k][N - 1] - u[k][N - 2];
  }
  //Step 4: reverse time back
  std::reverse(u.begin(), u.end());
  //Step 5:removing emissions
  for (int k = 0; k <= M; k += 100) {
    for (int j = 0; j <= N; j++) {
      if (u[k][j] < 0.00001)
        u[k][j] = 0;
    }
  }
  return u;
}