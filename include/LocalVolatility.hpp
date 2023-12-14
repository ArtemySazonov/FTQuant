/**
 * @file LocalVolatility.hpp
 * @brief Contains the definition of the Dupire's local volatility model class
 *
 * @section DESCRIPTION
 *
 * The Dupire's local volatility model is a mathematical model used to model the price of
 * various assets. The model assumes that the price of heavily traded
 * assets follow a geometric Brownian motion with deterministic function of stock
 * price and time as volatility. When applied to a stock option, the model incorporates
 * the local price variation of the stock, the time value of money, the option's
 * strike price and the time to the option's expiry.
 *
 * @see https://en.wikipedia.org/wiki/Local_volatility
 * @see https://www.investopedia.com/terms/b/blackscholes.asp
 * @see https://www.investopedia.com/terms/e/europeanoption.asp
 *
 * @date 2023-12-09
 */

#ifndef FTQUANT_LOCALVOLATILITY_HPP
#define FTQUANT_LOCALVOLATILITY_HPP
#include <vector>

/** @brief Implements the Dupire's local volatility model
 *
 */
class LocalVolatility {
 private:
  double r;
  std::function<double(double,double)> sigma;

 public:
  LocalVolatility();
  LocalVolatility(double r, std::function<double(double,double)> &sigma) : r(r), sigma(sigma) {}

  int calibrate_dupire();
  std::vector<std::vector<double>> generate_paths(int n_paths);
};


#endif  //FTQUANT_LOCALVOLATILITY_HPP
