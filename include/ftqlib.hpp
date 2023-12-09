#ifndef FTQUANT_LIBRARY_HPP
#define FTQUANT_LIBRARY_HPP

#include <string>
#include <functional>
#include <vector>

#include "BlackSholes.hpp"
#include "LocalVolatility.hpp"
#include "HestonSV.hpp"

/** @brief A container for storing the result of a Monte-Carlo simulation.
 *
 */
class MonteCarloResult
{
public:
    MonteCarloResult(int result_code, int n_simulations, double error, double result)
    : result_code(result_code), n_simulations(n_simulations), error(error), result(result)
    {}

    std::string to_json() const
    {
        std::string json = "{";
        json += "\"result_code\": " + std::to_string(result_code) + ", ";
        json += "\"n_simulations\": " + std::to_string(n_simulations) + ", ";
        json += "\"error\": " + std::to_string(error) + ", ";
        json += "\"result\": " + std::to_string(result);
        json += "}";
        return json;
    }

private:
    int result_code;
    int n_simulations;

    double error;
    double result;
};

/** @brief A controller for derivatives pricing via a Monte-Carlo simulation.
 *
 */
template <typename T>
class MonteCarloPricer
{
private:
    T model;

public:
    MonteCarloPricer(T model)
    : model(model)
    {}

    [[maybe_unused]] MonteCarloResult estimate_price(std::function<double(double)> payoff, double absolute_error, double confidence_level = 0.95, int num_simulations_per_round = 1000)
    {
        const double t_critical = 1.96; // TODO: use a table of t-critical values

        int n_simulations = 0;
        int result_code = 0;

        double result = 0.;
        double error = 0.;

        auto results = std::vector<double>();
        auto sample_stds = std::vector<double>();

        do
        {
            double sum = 0;
            double sum2 = 0;
            for (int i = 0; i < num_simulations_per_round; ++i)
            {
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
            for (size_t i = 0; i < results.size(); ++i)
            {
                mean_of_means += results[i];
                mean_of_sample_stds += sample_stds[i];
            }
            mean_of_means /= static_cast<double>(results.size());
            mean_of_sample_stds /= static_cast<double>(sample_stds.size());

            error = t_critical * mean_of_sample_stds / sqrt(results.size());

            n_simulations += num_simulations_per_round;
            result = mean_of_means;
            result_code = error < absolute_error ? 0 : 1;
        } while (result_code != 0);

        return {result_code, n_simulations, error, result};
    }
};

#endif //FTQUANT_LIBRARY_HPP
