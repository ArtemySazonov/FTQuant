#include <ftqlib.hpp>
/**
 * @brief Estimate a fair price of a given derivative in a given model.
 *
 * This function estimates a fair price of a derivative with a given payoff using the given finite difference version
 * of an asset price model.
 *
 * @param payoff A payoff of a given derivative.
 * @param
 *
 * @returns @refitem MonteCarloResult
 */

template<class Model> MonteCarloResult price(std::function<std::vector<float>, float> payoff, Model model)
{
    return {};
}
