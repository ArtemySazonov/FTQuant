#include "RectBivariateSpline.hpp"

#include <iostream>

std::vector<std::vector<std::array<double, 16>>>
RectBivariateCubicSpline::get_coefs() const {
  auto coefs = this->a;
  return coefs;
}

int transpose(std::vector<std::vector<double>>& mat) {
  std::vector<std::vector<double>> new_mat;
  for (unsigned j = 0; j < mat[0].size(); ++j) {
    std::vector<double> new_line;
    for (unsigned i = 0; i < mat.size(); ++i) {
      new_line.push_back(mat[i][j]);
    }
    new_mat.push_back(new_line);
  }
  mat = new_mat;
  return 0;
}

std::vector<std::vector<double>> diff(const std::vector<double>& x,
                                      const std::vector<std::vector<double>>& f,
                                      const int dim = 1) {
  std::vector<std::vector<double>> der;
  unsigned Ny = f[0].size();
  unsigned Nx = f.size();
  double tang;
  double hi;

  if (dim == 0) {
    double px;

    for (unsigned j = 0; j < Ny; ++j) {
      std::vector<double> der_;
      hi = x[1] - x[0];
      tang = (f[1][j] - f[0][j]) / hi;
      px = (f[1][j] - f[0][j] + tang * hi) / (2. * hi);
      der_.push_back(px);
      for (unsigned i = 1; i < Nx - 1; ++i) {
        px = (f[i + 1][j] - f[i - 1][j]) / (x[i + 1] - x[i - 1]);
        der_.push_back(px);
      }
      hi = x[Nx - 1] - x[Nx - 2];
      tang = (f[Nx - 1][j] - f[Nx - 2][j]) / hi;
      px = (tang * hi + f[Nx - 1][j] - f[Nx - 2][j]) / (2. * hi);

      der_.push_back(px);
      der.push_back(der_);
    }
    transpose(der);
  }
  if (dim == 1) {
    double py;

    for (unsigned i = 0; i < Nx; ++i) {
      std::vector<double> der_;

      hi = x[1] - x[0];
      tang = (f[i][1] - f[i][0]) / hi;
      py = (f[i][1] - f[i][0] + tang * hi) / (2. * hi);
      der_.push_back(py);
      for (unsigned j = 1; j < Ny - 1; ++j) {
        py = (f[i][j + 1] - f[i][j - 1]) / (x[j + 1] - x[j - 1]);
        der_.push_back(py);
      }
      hi = x[Ny - 1] - x[Ny - 2];
      tang = (f[i][Ny - 1] - f[i][Ny - 2]) / hi;
      py = (f[i][Ny - 1] - f[i][Ny - 2] + tang * hi) / (2. * hi);
      der_.push_back(py);
      der.push_back(der_);
    }
  }

  return der;
}

std::array<std::vector<std::vector<double>>, 3>
RectBivariateCubicSpline::weighted_finite_derivative(
    const std::vector<double>& x, const std::vector<double>& y,
    const std::vector<std::vector<double>>& f) {
  std::array<std::vector<std::vector<double>>, 3> der;
  der[0] = diff(x, f, 0);
  der[1] = diff(y, f, 1);
  der[2] = diff(y, der[0], 1);
  return der;
}

void RectBivariateCubicSpline::fit(const std::vector<double>& x,
                                   const std::vector<double>& y,
                                   const std::vector<std::vector<double>>& f) {

  auto derivative = this->weighted_finite_derivative(x, y, f);
  this->x = x;
  this->y = y;
  std::vector<std::vector<std::array<double, 16>>> coefs;
  for (unsigned i = 0; i < x.size() - 1; ++i) {
    double dx = x[i + 1] - x[i];
    std::vector<std::array<double, 16>> coef;
    for (unsigned j = 0; j < y.size() - 1; ++j) {
      std::array<double, 16> x_;
      std::array<double, 16> alpha;

      double dy = y[j + 1] - y[j];
      double dxdy = dx * dy;

      x_[0] = f[i][j], x_[1] = f[i + 1][j], x_[2] = f[i][j + 1],
      x_[3] = f[i + 1][j + 1];
      x_[4] = derivative[0][i][j] * dx, x_[5] = derivative[0][i + 1][j] * dx,
      x_[6] = derivative[0][i][j + 1] * dx,
      x_[7] = derivative[0][i + 1][j + 1] * dx;
      x_[8] = derivative[1][i][j] * dy, x_[9] = derivative[1][i + 1][j] * dy,
      x_[10] = derivative[1][i][j + 1] * dy,
      x_[11] = derivative[1][i + 1][j + 1] * dy;
      x_[12] = derivative[2][i][j] * dxdy,
      x_[13] = derivative[2][i + 1][j] * dxdy,
      x_[14] = derivative[2][i][j + 1] * dxdy,
      x_[15] = derivative[2][i + 1][j + 1] * dxdy;

      alpha[0] = x_[0], alpha[1] = x_[4],
      alpha[2] = 3 * (x_[1] - x_[0]) - 2. * x_[4] - x_[5];
      alpha[3] = 2. * (x_[0] - x_[1]) + x_[4] + x_[5], alpha[4] = x_[8],
      alpha[5] = x_[12];
      alpha[6] = 3. * (x_[9] - x_[8]) - 2. * x_[12] - x_[13],
      alpha[7] = 2. * (x_[8] - x_[9]) + x_[12] + x_[13];
      alpha[8] = 3. * (x_[2] - x_[0]) - 2. * x_[8] - x_[10],
      alpha[9] = 3. * (x_[6] - x_[4]) - 2. * x_[12] - x_[14];
      alpha[10] = 9. * (x_[0] - x_[1] - x_[2] + x_[3]) +
                  6. * (x_[4] - x_[6] + x_[8] - x_[9]) +
                  3. * (x_[5] - x_[7] + x_[10] - x_[11]) + 4. * x_[12] +
                  2. * (x_[13] + x_[14]) + x_[15];
      alpha[11] = 6. * (-x_[0] + x_[1] + x_[2] - x_[3]) +
                  3. * (-x_[4] - x_[5] + x_[6] + x_[7]) +
                  4. * (-x_[8] + x_[9]) +
                  2. * (x_[11] - x_[10] - x_[12] - x_[13]) - x_[14] - x_[15];
      alpha[12] = 2. * (x_[0] - x_[2]) + x_[8] + x_[10],
      alpha[13] = 2. * (x_[4] - x_[6]) + x_[12] + x_[14];
      alpha[14] = 6. * (-x_[0] + x_[1] + x_[2] - x_[3]) +
                  4. * (-x_[4] + x_[6]) + 2. * (-x_[5] + x_[7]) +
                  3. * (-x_[8] + x_[9] - x_[10] + x_[11]) +
                  2. * (-x_[12] - x_[14]) - x_[13] - x_[15];
      alpha[15] = 4. * (x_[0] - x_[1] - x_[2] + x_[3]) +
                  2. * (x_[4] + x_[5] - x_[6] - x_[7] + x_[8] - x_[9] + x_[10] -
                        x_[11]) +
                  x_[12] + x_[13] + x_[14] + x_[15];

      coef.push_back(alpha);
    }
    coefs.push_back(coef);
  }
  this->a = coefs;
  return;
}

int binsearch(const std::vector<double>& x, const double value) {

  if (value >= x[x.size() - 1])
    return x.size() - 1;
  if (value <= x[0])
    return -1;

  int RB = x.size() - 1;
  int LB = 0;
  int mid;
  double d;

  while (LB <= RB) {
    mid = (LB + RB) >> 1;
    d = x[mid];
    if (value < d && value >= x[mid - 1])
      return mid - 1;
    else if (value >= d && value < x[mid + 1])
      return mid;
    else if (d < value)
      LB = mid + 1;
    else
      RB = mid - 1;
  }
  return 0;
}

double RectBivariateCubicSpline::eval(double x_, double y_) const {
  int i0 = binsearch(this->x, x_);
  int j0 = binsearch(this->y, y_);
  double x_ev, y_ev;
  if (x_ <= this->x[0]) {
    x_ev = this->x[0], i0 = 0;
  } else if (x_ >= this->x[this->x.size() - 1]) {
    x_ev = this->x[this->x.size() - 1], i0 = this->x.size() - 2;
  } else {
    x_ev = x_;
  }
  if (y_ <= this->y[0]) {
    y_ev = this->y[0], j0 = 0;
  } else if (y_ >= this->y[this->y.size() - 1]) {
    y_ev = this->y[this->y.size() - 1], j0 = this->y.size() - 2;
  } else {
    y_ev = y_;
  }
  auto coef = this->a[i0][j0];
  double dx = this->x[i0 + 1] - this->x[i0];
  double dy = this->y[j0 + 1] - this->y[j0];
  double xbar = (x_ev - this->x[i0]) / dx;
  double ybar = (y_ev - this->y[j0]) / dy;

  std::array<double, 4> xi{1., xbar, xbar * xbar, xbar * xbar * xbar};
  std::array<double, 4> yi{1., ybar, ybar * ybar, ybar * ybar * ybar};

  double sum = 0;
  // std::cout<<"i0: "<<i0<<"    j0: "<<  j0 <<"   x_ev: "<<x_ev<<"   y_ev: "<<y_ev<<'\n';
  // std::cout<<"xbar: "<<xbar<<"  ybar: "<<ybar<<'\n';

  for (int j = 0; j < 4; ++j)
    for (int i = 0; i < 4; ++i)
      sum += coef[4 * j + i] * xi[i] * yi[j];
  return sum;
}

double RectBivariateCubicSpline::derivative_x(double x_, double y_) const {
  int i0 = binsearch(this->x, x_);
  int j0 = binsearch(this->y, y_);
  double x_ev, y_ev;
  if (x_ <= this->x[0]) {
    x_ev = this->x[0], i0 = 0;
  } else if (x_ >= this->x[this->x.size() - 1]) {
    x_ev = this->x[this->x.size() - 1], i0 = this->x.size() - 2;
  } else {
    x_ev = x_;
  }
  if (y_ <= this->y[0]) {
    y_ev = this->y[0], j0 = 0;
  } else if (y_ >= this->y[this->y.size() - 1]) {
    y_ev = this->y[this->y.size() - 1], j0 = this->y.size() - 2;
  } else {
    y_ev = y_;
  }

  auto coef = this->a[i0][j0];
  double dx = this->x[i0 + 1] - this->x[i0];
  double dy = this->y[j0 + 1] - this->y[j0];
  double xbar = (x_ev - this->x[i0]) / dx;
  double ybar = (y_ev - this->y[j0]) / dy;

  std::array<double, 4> xi{0., 1. / dx, 2. * xbar / dx, 3. * xbar * xbar / dx};
  std::array<double, 4> yi{1., ybar, ybar * ybar, ybar * ybar * ybar};

  double sum = 0;

  for (int j = 0; j < 4; ++j)
    for (int i = 1; i < 4; ++i)
      sum += coef[4 * j + i] * xi[i] * yi[j];
  return sum;
}

double RectBivariateCubicSpline::derivative_y(double x_, double y_) const {
  int i0 = binsearch(this->x, x_);
  int j0 = binsearch(this->y, y_);
  double x_ev, y_ev;
  if (x_ <= this->x[0]) {
    x_ev = this->x[0], i0 = 0;
  } else if (x_ >= this->x[this->x.size() - 1]) {
    x_ev = this->x[this->x.size() - 1], i0 = this->x.size() - 2;
  } else {
    x_ev = x_;
  }
  if (y_ <= this->y[0]) {
    y_ev = this->y[0], j0 = 0;
  } else if (y_ >= this->y[this->y.size() - 1]) {
    y_ev = this->y[this->y.size() - 1], j0 = this->y.size() - 2;
  } else {
    y_ev = y_;
  }

  auto coef = this->a[i0][j0];
  double dx = this->x[i0 + 1] - this->x[i0];
  double dy = this->y[j0 + 1] - this->y[j0];
  double xbar = (x_ev - this->x[i0]) / dx;
  double ybar = (y_ev - this->y[j0]) / dy;

  std::array<double, 4> xi{1., xbar, xbar * xbar, xbar * xbar * xbar};
  std::array<double, 4> yi{0., 1. / dy, 2. * ybar / dy, 3. * ybar * ybar / dy};

  double sum = 0.;

  for (int j = 1; j < 4; ++j)
    for (int i = 0; i < 4; ++i)
      sum += coef[4 * j + i] * xi[i] * yi[j];
  return sum;
}

double RectBivariateCubicSpline::derivative_xx(double x_, double y_) const {
  int i0 = binsearch(this->x, x_);
  int j0 = binsearch(this->y, y_);
  double x_ev, y_ev;
  if (x_ <= this->x[0]) {
    x_ev = this->x[0], i0 = 0;
  } else if (x_ >= this->x[this->x.size() - 1]) {
    x_ev = this->x[this->x.size() - 1], i0 = this->x.size() - 2;
  } else {
    x_ev = x_;
  }
  if (y_ <= this->y[0]) {
    y_ev = this->y[0], j0 = 0;
  } else if (y_ >= this->y[this->y.size() - 1]) {
    y_ev = this->y[this->y.size() - 1], j0 = this->y.size() - 2;
  } else {
    y_ev = y_;
  }

  auto coef = this->a[i0][j0];
  double dx = this->x[i0 + 1] - this->x[i0];
  double dy = this->y[j0 + 1] - this->y[j0];
  double xbar = (x_ev - this->x[i0]) / dx;
  double ybar = (y_ev - this->y[j0]) / dy;

  std::array<double, 4> xi{0., 0., 2. / (dx * dx), 6. * xbar / (dx * dx)};
  std::array<double, 4> yi{1., ybar, ybar * ybar, ybar * ybar * ybar};

  double sum = 0;

  for (int j = 0; j < 4; ++j)
    for (int i = 2; i < 4; ++i)
      sum += coef[4 * j + i] * xi[i] * yi[j];
  return sum;
}

double RectBivariateCubicSpline::derivative_yy(double x_, double y_) const {
  int i0 = binsearch(this->x, x_);
  int j0 = binsearch(this->y, y_);
  double x_ev, y_ev;
  if (x_ <= this->x[0]) {
    x_ev = this->x[0], i0 = 0;
  } else if (x_ >= this->x[this->x.size() - 1]) {
    x_ev = this->x[this->x.size() - 1], i0 = this->x.size() - 2;
  } else {
    x_ev = x_;
  }
  if (y_ <= this->y[0]) {
    y_ev = this->y[0], j0 = 0;
  } else if (y_ >= this->y[this->y.size() - 1]) {
    y_ev = this->y[this->y.size() - 1], j0 = this->y.size() - 2;
  } else {
    y_ev = y_;
  }

  auto coef = this->a[i0][j0];
  double dx = this->x[i0 + 1] - this->x[i0];
  double dy = this->y[j0 + 1] - this->y[j0];
  double xbar = (x_ev - this->x[i0]) / dx;
  double ybar = (y_ev - this->y[j0]) / dy;

  std::array<double, 4> xi{1., xbar, xbar * xbar, xbar * xbar * xbar};
  std::array<double, 4> yi{0., 0., 2. / (dy * dy), 6. * ybar / (dy * dy)};

  double sum = 0;

  for (int j = 2; j < 4; ++j)
    for (int i = 0; i < 4; ++i)
      sum += coef[4 * j + i] * xi[i] * yi[j];
  return sum;
}

double RectBivariateCubicSpline::derivative_xy(double x_, double y_) const {
  int i0 = binsearch(this->x, x_);
  int j0 = binsearch(this->y, y_);
  double x_ev, y_ev;
  if (x_ <= this->x[0]) {
    x_ev = this->x[0], i0 = 0;
  } else if (x_ >= this->x[this->x.size() - 1]) {
    x_ev = this->x[this->x.size() - 1], i0 = this->x.size() - 2;
  } else {
    x_ev = x_;
  }
  if (y_ <= this->y[0]) {
    y_ev = this->y[0], j0 = 0;
  } else if (y_ >= this->y[this->y.size() - 1]) {
    y_ev = this->y[this->y.size() - 1], j0 = this->y.size() - 2;
  } else {
    y_ev = y_;
  }

  auto coef = this->a[i0][j0];
  double dx = this->x[i0 + 1] - this->x[i0];
  double dy = this->y[j0 + 1] - this->y[j0];
  double xbar = (x_ev - this->x[i0]) / dx;
  double ybar = (y_ev - this->y[j0]) / dy;

  std::array<double, 4> xi{0., 1. / dx, 2. * xbar / dx, 3. * xbar * xbar / dx};
  std::array<double, 4> yi{0., 1. / dy, 2. * ybar / dy, 3. * ybar * ybar / dy};

  double sum = 0.;

  for (int j = 1; j < 4; ++j)
    for (int i = 1; i < 4; ++i)
      sum += coef[4 * j + i] * xi[i] * yi[j];
  return sum;
}

double RectBivariateCubicSpline::partial_derivative(double x_, double y_,
                                                    int dx, int dy) const {
  if (dx == 1 && dy == 0)
    return derivative_x(x_, y_);
  if (dx == 2 && dy == 0)
    return derivative_xx(x_, y_);
  if (dx == 0 && dy == 1)
    return derivative_y(x_, y_);
  if (dx == 0 && dy == 2)
    return derivative_yy(x_, y_);
  if (dx == 1 && dy == 1)
    return derivative_xy(x_, y_);
  std::cout << "dx + dy must be less than 2"
            << "\n";
  return -1.;
}