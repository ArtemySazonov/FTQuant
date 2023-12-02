#ifndef FTQUANT_LIBRARY_HPP
#define FTQUANT_LIBRARY_HPP

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
    int result_code;

private:
    int n_simulations;
    double sample_std;
    double result;
};

#endif //FTQUANT_LIBRARY_HPP
