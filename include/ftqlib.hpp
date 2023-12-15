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

#include <cmath>
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

  /** A function that estimates the price of a given derivative via a Monte-Carlo simulation with a chosen method.
   * @brief Estimates the price of a derivative via a Monte-Carlo simulation.
   * @param payoff The payoff function of the derivative.
   * @param absolute_error The absolute error of the simulation we want to have.
   * @param confidence_level The confidence level of the simulation we want to have.
   * @param num_simulations_per_round The number of Monte-Carlo simulations per round.
   * @return A MonteCarloResult object.
   * @see MonteCarloResult
   * @see MonteCarloPricer
   */
  MonteCarloResult estimate_price(std::function<double(double)> payoff,
                                  double absolute_error, int steps, double time,
                                  double spot, bool antithetic = true,
                                  double confidence_level = 0.95,
                                  int num_simulations_per_round = 1000) {

    const double t_critical = 1.96;  // TODO: use a table of t-critical values

    int n_simulations = 0;
    int n_iterations = 0;
    int result_code = ERROR_NO_CONVERGENCE;

    double result = 0.;
    double error = 0.;
    double result_var = 0.;

    do {
      double sum = 0;
      double sum2 = 0;

      ++n_iterations;
      n_simulations += num_simulations_per_round;

      auto paths = model.generate_paths(num_simulations_per_round, steps, time,
                                        spot, false);
      for (auto path : paths) {
        double payoff_value = payoff(path.back());
        sum += payoff_value;
        sum2 += payoff_value * payoff_value;
      }

      auto mean = sum / num_simulations_per_round;
      auto mean2 = sum2 / num_simulations_per_round;
      auto sample_var = (mean2 - mean * mean);  // / num_simulations_per_round;

      result_var =
          ((n_iterations - 1) * result_var + sample_var) / (n_iterations);
      result = (n_iterations * result + mean) / (n_iterations + 1);

      error = t_critical * abs(result_var) / sqrt(n_simulations);

      result_code = error < absolute_error ? SUCCESS : ERROR_NO_CONVERGENCE;
    } while (result_code != SUCCESS && n_simulations < 10000000);

    return {result_code, n_simulations, error, result};
  }
};

/** @class PartialDiffEqPricer
 *  @brief A controller for derivatives pricing via a PDE solution.
 *  @deprecated This class is deprecated and will be removed in the future.
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
