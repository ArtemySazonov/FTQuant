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

#ifndef FTQUANT_BLACKSHOLES_HPP
#define FTQUANT_BLACKSHOLES_HPP
#include <vector>

/**
 * @class BlackSholes
 * @brief Implements the Black-Sholes model
 */
class BlackSholes {
 private:
  double r;
  double sigma;

 public:
  BlackSholes(double r, double sigma) : r(r), sigma(sigma) {}

  std::vector<std::vector<double>> generate_paths(int n_paths);
};

#endif  //FTQUANT_BLACKSHOLES_HPP
