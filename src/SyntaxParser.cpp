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

#include <gtest/gtest.h>

#include "../include/SyntaxParser.hpp"

bool isDouble(const std::string& str) {
  std::istringstream iss(str);
  double d;
  iss >> std::noskipws >> d;
  return iss.eof() && !iss.fail();
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
    return;
  }

  while (ss >> word) {
    words.push_back(word);
  }

  for (const auto& w : words) {
    // std::cout << w << '\n';

    if (Fields.count(w)) {
      if (!flag) {
        _key_numbers[w] = 0;
        flag = 1;
        temp = w;
      }
      continue;
    }

    if (flag == 1) {
      if (isDouble(w))
        _key_numbers[temp] = std::stod(w);
      else {
        std::cout << "cant convert " << w << " in double \n";
        _code = INVALID_COMMAND;
        return;
      }
      flag = 0;
    }
  }

  for (const auto& w : RequiredFields[_code]) {
    if (!_key_numbers.count(w)) {
      _code = INVALID_COMMAND;

      // std::cout << "bed things: there is not " << w <<'\n';
    }
  }
}

int Command::code() const {
  return _code;
}

int Command::execute() const {
  double r = 0;
  double sigma = 1;
  int trajectories_generated = 0;
  std::vector<std::vector<double>> traj;
  std::cout << "BlackScholes model(r, sigma); \n";
  switch (_code) {
    case INVALID_COMMAND:
      return -1;
      break;

    case BLACK_SCHOLES:
      std::cout << "BlackScholes model(_key_numbers[\"INTEREST_RATE\"], "
                   "_key_numbers[\"SIGMA\"]); \n";
      break;

    case GENERATE_TRAJECTORIES:
      std::cout << "traj = "
                   "model.generateTrajectories(_key_numbers[\"TRAJECTORIES_"
                   "NUMBER\"], _key_numbers[\"STEPS_NUMBER\"]); \n";
      trajectories_generated = 1;
      break;

    case EURO_PUT:
      std::cout << "EuroPut(_key_numbers[\"STOCK_PRICE\"], "
                   "_key_numbers[\"STRIKE\"]) \n";
      if (!trajectories_generated) {
        std::cout << "model.generateTrajectories(100, 1000)\n";
      }
      std::cout << "EuroPut.price(traj) \n";
      break;

    case EURO_CALL:
      std::cout << "EuroPut(_key_numbers[\"STOCK_PRICE\"], "
                   "_key_numbers[\"STRIKE\"]) \n";
      if (!trajectories_generated) {
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

std::string Command::to_json() const {
  std::string json = "{";
  json += "code: " + std::to_string(_code) + ", " + "keyNumbers: ";
  for (const auto& [key, value] : _key_numbers)
    json += '[' + key + "] = " + std::to_string(value) + "; ";

  json += '}';

  return json;
}

std::ostream& operator<<(std::ostream& os, const Command& C) {
  std::cout << "code: " << C._code << std::endl;
  std::cout << "keyNumbers: ";
  for (const auto& [key, value] : C._key_numbers)
    std::cout << '[' << key << "] = " << value << "; ";

  std::cout << std::endl;

  return os;
}

#endif  //FTQUANT_SYNTAXPARSER_HPP
