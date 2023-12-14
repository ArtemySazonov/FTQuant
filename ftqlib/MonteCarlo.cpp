/**
 * @file MonteCarlo.cpp
 * This file contains the implementation of the MonteCarloResult and MonteCarloPricer classes.
 * @brief Implementations of the MonteCarloResult and MonteCarloPricer classes.
 */
#include <ftqlib.hpp>

/** A function that converts a MonteCarloResult to a JSON string.
 *  @brief MonteCarloResult to a JSON string.
 *  @return A JSON string.
 *  @see MonteCarloResult
 */
std::string MonteCarloResult::to_json() const {
  std::string json = "{";
  json += "\"result_code\": " + std::to_string(result_code) + ", " +
          "\"n_simulations\": " + std::to_string(n_simulations) + ", " +
          "\"error\": " + std::to_string(error) + ", " +
          "\"result\": " + std::to_string(result) + "}";

  return json;
}

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
template <typename T>
MonteCarloResult MonteCarloPricer<T>::estimate_price(const std::function<double(std::vector<double>)>& payoff,
                                  double absolute_error,
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

    auto paths = model.generate_paths(num_simulations_per_round);
    for (auto path : paths) {
      double payoff_value = payoff(path);
      sum += payoff_value;
      sum2 += payoff_value * payoff_value;
    }

    auto mean = sum / num_simulations_per_round;
    auto mean2 = sum2 / num_simulations_per_round;
    auto sample_var = (mean2 - mean * mean) / num_simulations_per_round;

    result_var =
        ((n_iterations - 1) * result_var + sample_var) / (n_iterations);
    result = (n_iterations * result + mean) / (n_iterations + 1);
    ++n_iterations;
    n_simulations += num_simulations_per_round;

    error = t_critical * abs(result_var) / sqrt(n_simulations);

    result_code = error < absolute_error ? SUCCESS : ERROR_NO_CONVERGENCE;
  } while (result_code != SUCCESS && n_simulations < 10000000);

  return {result_code, n_simulations, error, result};
}
