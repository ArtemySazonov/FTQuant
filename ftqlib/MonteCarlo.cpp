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
  json += "\"result_code\": " + std::to_string(result_code) + ", ";
  json += "\"n_simulations\": " + std::to_string(n_simulations) + ", ";
  json += "\"error\": " + std::to_string(error) + ", ";
  json += "\"result\": " + std::to_string(result);
  json += "}";
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
MonteCarloResult MonteCarloPricer<T>::estimate_price(
    const std::function<double(double)>& payoff, double absolute_error,
    double confidence_level, int num_simulations_per_round) {

  const double t_critical = 1.96;  // TODO: use a table of t-critical values

  int n_simulations = 0;
  int result_code = ERROR_NO_CONVERGENCE;

  double result = 0.;
  double error = 0.;

  auto results = std::vector<double>();
  auto sample_stds = std::vector<double>();

  do {
    double sum = 0;
    double sum2 = 0;
    for (int i = 0; i < num_simulations_per_round; ++i) {
      double price = model.simulate_price();
      double payoff_value = payoff(price);
      sum += payoff_value;
      sum2 += payoff_value * payoff_value;
    }

    double mean = sum / num_simulations_per_round;
    double mean2 = sum2 / num_simulations_per_round;
    double std = sqrt(mean2 - mean * mean);
    double sample_std = std / sqrt(num_simulations_per_round);

    results.push_back(mean);
    sample_stds.push_back(sample_std);

    double mean_of_means = 0;
    double mean_of_sample_stds = 0;
    for (size_t i = 0; i < results.size(); ++i) {
      mean_of_means += results[i];
      mean_of_sample_stds += sample_stds[i];
    }
    mean_of_means /= static_cast<double>(results.size());
    mean_of_sample_stds /= static_cast<double>(sample_stds.size());

    error = t_critical * abs(mean_of_sample_stds) / sqrt(results.size());

    n_simulations += num_simulations_per_round;
    result = mean_of_means;
    result_code = error < absolute_error ? SUCCESS : ERROR_NO_CONVERGENCE;
  } while (result_code != SUCCESS && n_simulations < 1000000);

  return {result_code, n_simulations, error, result};
}
