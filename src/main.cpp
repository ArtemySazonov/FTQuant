#include <SyntaxParser.hpp>

/** @mainpage
 * FTQuant is a derivative pricing terminal using Monte-Carlo simulations with it's own language and ability to solve PDEs.
 *
 * Possible ways to improve the implementation:
 *
 * 1. Multithreaded Monte-Carlo;
 *
 * 2. More feature-rich set of derivatives (e.g. structural products like autocall);
 *
 * 3. More models (Heston stochastic volatility model, local stochastic volatility models).
 *
 * Library usage:
 * ```cpp
 * int main ()
 * {
 *     Execution Ex;
 *
 *     Command C("BLACK_SCHOLES INTEREST_RATE 0.14 SIGMA 0.8 EXP_T 1. SPOT_PRICE 100.");
 *     std::cout << Ex.execute(C) << std::endl;
 *
 *     Command B1("EURO_CALL ERROR 0.01 STRIKE_PRICE 100 TRAJECTORIES_NUMBER 100 STEPS_NUMBER 100 EXP_T 1. SPOT_PRICE 100.");
 *     std::cout << Ex.execute(B1) << std::endl;
 *
 *     return 0;
 * }
 * ```
 */
int main() {
  Execution Ex;
  while (true)
  {
    std::string cmd;
    std::getline(std::cin, cmd);

    if (cmd == "EXIT") break;

    Command C(cmd);

    std::cout << C.to_json() << std::endl;
    std::cout << Ex.execute(C) << std::endl;
  }
  return 0;
}