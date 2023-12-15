/** @file PartialDiffEq.cpp
 *  This file contains the implementation of the PartialDiffEqPricer methods.
 *  @brief Implementation of PartialDiffEqPricer class.
 *  @see PartialDiffEqPricer
 *  @see MonteCarloPricer
 *  @see tests/u_PartialDiffEq.cpp
 *  @deprecated This class is deprecated and will be removed in the future.
 */
#include "ftqlib.hpp"

template <typename T>
double PartialDiffEqPricer<T>::calculate_price(
    const std::function<double(double)>& payoff, double absolute_error) {
  return 0;
}