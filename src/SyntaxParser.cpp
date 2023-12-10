/** @file SyntaxParser.cpp
 * This file contains the implementation of the SyntaxParser methods.
 * @brief Implementation of SyntaxParser class.
 * @see SyntaxParser
 * @see tests/u_SyntaxParser.cpp
 */

#ifndef FTQUANT_SYNTAXPARSER_CPP
#define FTQUANT_SYNTAXPARSER_CPP

#include <stdio.h>
#include <string.h>
#include <iostream>
#include <map>
#include <sstream>
#include <string>
#include <string_view>
#include <vector>

#include "../include/SyntaxParser.hpp"

bool isDouble(const std::string& str) {
  std::istringstream iss(str);
  double d;
  iss >> std::noskipws >>
      d;  // Use noskipws to ensure entire string is consumed
  return iss.eof() &&
         !iss.fail();  // Check if entire string was consumed and no error occurred
}

Command::Command(std::string com) {
    std::stringstream ss(com);
    std::vector<std::string> words;

    std::string temp;

    int flag = 0;
    std::string word;
    ss >> word;
    if (Commands.count(word)) {
      _code = Commands[word];
    } else {
      _code = Commands["INVALID_COMMAND"];
    }

    while (ss >> word) {
      words.push_back(word);
    }

    for (const auto& w : words) {
      // std::cout << w << '\n';

      if (Fields.count(w)) {
        if (!flag) {
          _keyNumbers[w] = 0;
          flag = 1;
          temp = w;
        }
        continue;
      }

      if (flag == 1) {
        if (isDouble(w))
          _keyNumbers[temp] = std::stod(w);
        else {
          std::cout << "cant convert " << w << " in double \n";
        }
        flag = 0;
      }
    }

    for (const auto& w : RequiredFields[_code]){
        if(!_keyNumbers.count(w)){
            _code = INVALID_COMMAND;

            // std::cout << "bed things: there is not " << w <<'\n';
        }
    }
  }

int Command::execute()
{
    double r = 0;
    double sigma = 1;
    int trajectories_generated = 0;
    std::vector<std::vector<double>> traj;
    std::cout << "BlackScholes model(r, sigma); \n";
    switch (_code)
    {
    case INVALID_COMMAND:
        return -1;
        break;

    case BLACK_SCHOLES:
        std::cout << "BlackScholes model(_keyNumbers[\"INTEREST_RATE\"], _keyNumbers[\"SIGMA\"]); \n";
        break;
    
    case GENERATE_TRAJECTORIES:
        std::cout << "traj = model.generateTrajectories(_keyNumbers[\"TRAJECTORIES_NUMBER\"], _keyNumbers[\"STEPS_NUMBER\"]); \n";
        trajectories_generated = 1;
        break;

    case EURO_PUT:
        std::cout << "EuroPut(_keyNumbers[\"STOCK_PRICE\"], _keyNumbers[\"STRIKE\"]) \n";
        if(!trajectories_generated) {
            std::cout << "model.generateTrajectories(100, 1000)\n";
        }
        std::cout << "EuroPut.price(traj) \n";
        break;

    case EURO_CALL:
        std::cout << "EuroPut(_keyNumbers[\"STOCK_PRICE\"], _keyNumbers[\"STRIKE\"]) \n";
        if(!trajectories_generated) {
            std::cout << "model.generateTrajectories(100, 1000)\n";
        }
        std::cout << "EuroPut.price(traj) \n";
        break;
    
    default:
        return -1;
        break;
    }
    return 0;
}

std::ostream &operator<<(std::ostream& os, const Command& C) {
    std::cout << "code: " << C._code << std::endl;
    std::cout << "keyNumbers: ";
    for (const auto& [key, value] : C._keyNumbers)
      std::cout << '[' << key << "] = " << value << "; ";

    std::cout << std::endl;

    return os;
  }


int main(void)
{
    Command C("GENERATE_TRAJECTORIES SIGMA 5 INTEREST_RATE 6");
    std::cout << C;
    C.execute();

    std::cout << std::endl;

    return 0;
}

#endif  //FTQUANT_SYNTAXPARSER_HPP
