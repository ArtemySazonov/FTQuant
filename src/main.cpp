#include <SyntaxParser.hpp>

int main() {
  Command C(
      "BLACK_SCHOLES INTEREST_RATE 0.14 SIGMA 0.8 EXP_T 1. SPOT_PRICE 1.");

  std::cout << C.to_json() << std::endl;

  Execution Ex;

  std::cout << Ex.execute(C) << std::endl;

  Command B1(
      "EURO_CALL ERROR 0.01 STRIKE_PRICE 100 TRAJECTORIES_NUMBER 100 STEPS_NUMBER 100 EXP_T 1. SPOT_PRICE 100.");

  std::cout << B1.to_json() << std::endl;

  std::cout << Ex.execute(B1) << std::endl;


  Command B2(
      "EURO_CALL ERROR 0.01 STRIKE_PRICE 100 TRAJECTORIES_NUMBER 1000 STEPS_NUMBER 10 EXP_T 1. SPOT_PRICE 100.");

  std::cout << B2.to_json() << std::endl;

  std::cout << Ex.execute(B2) << std::endl;


  Command B3(
      "EURO_PUT ERROR 0.01 STRIKE_PRICE 100 TRAJECTORIES_NUMBER 100 STEPS_NUMBER 100 EXP_T 1. SPOT_PRICE 100.");

  std::cout << B3.to_json() << std::endl;

  std::cout << Ex.execute(B3) << std::endl;
  return 0;
}