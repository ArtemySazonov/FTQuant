/**
 * @file ftqlib.hpp
 * @brief The main header file for the FTQlib library.
 *
 * This file contains the main header file for the FTQlib library. It contains
 * the main classes and functions for the library.
 *
 * @see include/BlackSholes.hpp
 * @see include/LocalVolatility.hpp
 * @see include/MonteCarlo.hpp
 *
 * @date 2023-12-09
 */
#ifndef FTQUANT_LIBRARY_HPP
#define FTQUANT_LIBRARY_HPP

#include <functional>
#include <string>
#include <vector>

#include "BlackSholes.hpp"
#include "LocalVolatility.hpp"

[[maybe_unused]] const int ERROR_MATHEMATICS =
    -1; /**< Error code for any mathematical errors. */
[[maybe_unused]] const int SUCCESS = 0; /**< Success code. */
[[maybe_unused]] const int ERROR_NO_CONVERGENCE =
    1; /**< Error code for lack of convergence in simulations and other numerical estimations. */
[[maybe_unused]] const int ERROR_INVALID_INPUT =
    2; /**< Error code for invalid input-related issues. */
[[maybe_unused]] const int ERROR_INVALID_MODEL =
    3; /**< Error code for model-related issues. */

/** @class MonteCarloResult
 *  @brief A container for storing the result of a Monte-Carlo simulation.
 *
 *  @param result_code The result code of the simulation.
 *  @param n_simulations The number of simulations performed.
 *  @param error The error of the simulation.
 *  @param result The result of the simulation.
 */
class MonteCarloResult {
 public:
  MonteCarloResult(int result_code, int n_simulations, double error,
                   double result)
      : result_code(result_code),
        n_simulations(n_simulations),
        error(error),
        result(result) {}

  [[nodiscard]] std::string to_json() const;

 private:
  int result_code;
  int n_simulations;

  double error;
  double result;
};

/** @class MonteCarloPricer
 *  @brief A controller for derivatives pricing via a Monte-Carlo simulation.
 *
 */
template <typename T>
class MonteCarloPricer {
 private:
  T model;

 public:
  MonteCarloPricer(T model) : model(model) {}

  MonteCarloResult estimate_price(const std::function<double(std::vector<double>)>& payoff,
                                  double absolute_error,
                                  double confidence_level = 0.95,
                                  int num_simulations_per_round = 1000);
};

/** @class PartialDiffEqPricer
 *  @brief A controller for derivatives pricing via a PDE solution.
 *
 */
template <typename T>
class PartialDiffEqPricer {
 private:
  T model;

 public:
  PartialDiffEqPricer(T model) : model(model) {}

  double calculate_price(const std::function<double(double)>& payoff,
                         double absolute_error);
};

#endif  //FTQUANT_LIBRARY_HPP
