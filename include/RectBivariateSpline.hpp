#ifndef FTQUANT_RECTBIVARIATESPLINE_HPP
#define FTQUANT_RECTBIVARIATESPLINE_HPP

#include <array>
#include <vector>

/** @class RectBivariateCubicSpline
 * @brief Implementation of 2d spline interpolation on a rectangular grid.
 * @see tests/u_RectBivariateSpline.cpp
 */
class RectBivariateCubicSpline {
 private:
  std::vector<double> x;
  std::vector<double> y;
  std::vector<std::vector<std::array<double, 16>>> a;

 public:
  RectBivariateCubicSpline(){};
  std::array<std::vector<std::vector<double>>, 3> weighted_finite_derivative(
      const std::vector<double>& x, const std::vector<double>& y,
      const std::vector<std::vector<double>>& f);
  void fit(const std::vector<double>& x, const std::vector<double>& y,
           const std::vector<std::vector<double>>& f);
  double eval(double x_, double y_) const;
  double derivative_x(double x_, double y_) const;
  double derivative_y(double x_, double y_) const;
  double derivative_xx(double x_, double y_) const;
  double derivative_yy(double x_, double y_) const;
  double derivative_xy(double x_, double y_) const;
  double partial_derivative(double x_, double y_, int dx, int dy) const;
  std::vector<std::vector<std::array<double, 16>>> get_coefs() const;
};

#endif  //FTQUANT_RECTBIVARIATESPLINE_HPP