#include <LocalVolatility.hpp>
#include <RectBivariateSpline.hpp>
#include <random>

LocalVolatility::LocalVolatility() : r(0.) {
  sigma = [](double a, double b) {
    return 0.;
  };
}

std::vector<std::vector<double>> LocalVolatility::generate_paths(int n_paths,
                                                                 double spot,
                                                                 int steps,
                                                                 double T) {
  std::random_device rd{};
  std::mt19937 gen(rd());

  std::vector<std::vector<double>> paths;
  std::normal_distribution<double> Normal(0, 1);
  double dt = T / steps;
  double sqdt = sqrt(dt);

  for (int i = 0; i < n_paths; ++i) {

    std::vector<double> path;
    path.push_back(0);
    double ti = 0.;

    for (int j = 1; j <= steps; ++j) {
      double sigma_val = this->sigma(ti, exp(path[j]));
      path.push_back(path[j - 1] +
                     (this->r - 0.5 * sigma_val * sigma_val) * dt +
                     sigma_val * sqdt * Normal(gen));
      ti += dt;
      path[j - 1] = spot * exp(path[j - 1]);
    }
    path[path.size() - 1] = spot * exp(path[path.size() - 1]);
    paths.push_back(path);
  }
  return paths;
}

int LocalVolatility::calibrate_dupire(std::vector<std::vector<double>>& omega,
                                      std::vector<double>& T,
                                      std::vector<double>& y, double spot) {
  RectBivariateCubicSpline w;
  w.fit(T, y, omega);

  auto glambda = [w, r, spot](double t, double k) {
    double y_ = log(k / exp(r * t) * spot);
    double w_ev = w.eval(t, y_);
    double Dx_w = w.partial_derivative(t, k, 0, 1);
    double D2x_w = w.partial_derivative(t, k, 0, 2);
    double Dt_w = w.partial_derivative(t, k, 1, 0);

    double y_over_w_ev = y_ / w_ev;
    double denom =
        1 - y_over_w_ev * Dx_w +
        0.25 * (-0.25 - 1. / w_ev + y_over_w_ev * y_over_w_ev) * Dx_w * Dx_w +
        0.5 * D2x_w;

    return sqrt(Dt_w / denom);
  };
  this->sigma = glambda;
  return 0;
}
