/**
 * @file BlackSholes.hpp
 * @brief Contains the definition of the Black-Sholes model
 *
 * @section DESCRIPTION
 *
 * The Black-Sholes model is a mathematical model used to model the price of
 * various assets. The model assumes that the price of heavily traded
 * assets follow a geometric Brownian motion with constant drift and volatility.
 * When applied to a stock option, the model incorporates the constant price
 * variation of the stock, the time value of money, the option's strike price
 * and the time to the option's expiry.
 *
 * @see https://en.wikipedia.org/wiki/Black%E2%80%93Scholes_model
 * @see https://www.investopedia.com/terms/b/blackscholes.asp
 * @see https://www.investopedia.com/terms/e/europeanoption.asp
 */

#ifndef FTQUANT_BLACKSCHOLES_HPP
#define FTQUANT_BLACKSCHOLES_HPP
#include <vector>
#include <functional>

void thomas_algorithm(std::vector<double>& v, std::vector<double>& a,
                      std::vector<double>& b, std::vector<double>& c,
                      std::vector<double>& f);

/**
 * @class BlackScholes
 * @brief Implements the Black-Scholes model
 */
class BlackScholes {
 private:
  double r;
  double sigma;

 public:
  BlackScholes() : r(0.), sigma(1.) {}

  BlackScholes(double r, double sigma) : r(r), sigma(sigma) {}

  std::vector<std::vector<double>> generate_paths(int n_paths, int steps,
                                                  double T, double spot,
                                                  bool antithetic = true);
  void calibrate(std::vector<double>& stock_prices);
  std::vector<std::vector<double>> pde_pricer(
      double r, double sigma, double T, std::function<double(double)> payoff,
      double S_max, double S_min, int M, int N);
};

#endif  //FTQUANT_BLACKSCHOLES_HPP
