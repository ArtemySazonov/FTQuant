#include <BlackSholes.hpp>
#include <vector>

/** Implementation of Thomas algorithm for tridiagonal matrices
 * @brief Thomas tridiagonal matrix algorithm
 * @param a,b,c,f matrix coefficients in the run-through method
 * @param v the desired approximation of the function obtained by the run-through method. Has indexes from 1 to n
 */
void thomas_algorithm(std::vector<double>& v, std::vector<double>& a,
                      std::vector<double>& b, std::vector<double>& c,
                      std::vector<double>& f) {
  std::size_t n = a.size() - 1;

  std::vector<double> alpha;
  alpha.resize(n + 1);

  std::vector<double> beta;
  beta.resize(n + 1);

  alpha[2] = b[1] / c[1];
  beta[2] = f[1] / c[1];

  for (std::size_t j = 2; j <= n - 1; j++) {
    alpha[j + 1] = b[j] / (c[j] - a[j] * alpha[j]);
    beta[j + 1] = (f[j] + a[j] * beta[j]) / (c[j] - a[j] * alpha[j]);
  }

  v[n] = (f[n] + a[n] * beta[n]) / (c[n] - a[n] * alpha[n]);

  for (std::size_t j = n - 1; j >= 1; j--) {
    v[j] = beta[j + 1] + alpha[j + 1] * v[j + 1];
  }
}