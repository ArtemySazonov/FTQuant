#include <SyntaxParser.hpp>

int main() {
  Command C(
      "BLACK_SCHOLES INTEREST_RATE 0.14 SIGMA 0.8 EXP_T 1. SPOT_PRICE 1.");

  std::cout << C.to_json() << std::endl;

  Execution Ex;

  std::cout << Ex.execute(C) << std::endl;

  // Command A(
  //     "GENERATE_TRAJECTORIES TRAJECTORIES_NUMBER 100 STEPS_NUMBER 10 EXP_T 1. "
  //     "SPOT_PRICE 1.");

  // std::cout << A.to_json() << std::endl;

  // std::cout << Ex.execute(A) << std::endl;

  Command B("EURO_CALL ERROR 0.01 STRIKE_PRICE 100 TRAJECTORIES_NUMBER 100 STEPS_NUMBER 1000 EXP_T 1. SPOT_PRICE 100.");

  std::cout << B.to_json() << std::endl;

  // std::cout << Ex.execute(B) << std::endl;
  return 0;
}