#ifndef FTQUANT_SYNTAXPARSER_HPP
#define FTQUANT_SYNTAXPARSER_HPP

#include <iostream>
#include <map>
#include <sstream>
#include <string>
#include <string_view>
#include <vector>

namespace {
std::map<std::string, int> Fields{
    {"INTEREST_RATE", 0}, {"DERIVATIVE", 1},   {"TRAJECTORIES_NUMBER", 2},
    {"NAME", 3},          {"STEPS_NUMBER", 4}, {"PARAMS", 5},
    {"SIGMA", 6}};

std::map<std::string, int> Commands{
    {"INVALID_COMMAND", -1},      {"BLACK_SCHOLES", 0}, {"LOCVOL", 1},
    {"GENERATE_TRAJECTORIES", 2}, {"EURO_PUT", 3},      {"EURO_CALL", 4}};

enum {
  INVALID_COMMAND = -1,
  BLACK_SCHOLES = 0,
  LOCVOL = 1,
  GENERATE_TRAJECTORIES = 2,
  EURO_PUT = 3,
  EURO_CALL = 4
};

std::vector<std::vector<std::string>> RequiredFields{
    {"INTEREST_RATE", "SIGMA"},
    {},
    {"TRAJECTORIES_NUMBER", "STEPS_NUMBER"},
    {"STOCK_PRICE", "STRIKE_PRICE"},
    {"STOCK_PRICE", "STRIKE_PRICE"}};
}  // namespace

bool isDouble(const std::string& str);

class Command {
 private:
  int _code;
  std::map<std::string, double> _key_numbers;

 public:
  Command() : _code(Commands["INVALID_COMMAND"]) {}

  explicit Command(std::string com);

  int execute() const;

  int code() const;

  std::string to_json() const;

  friend std::ostream& operator<<(std::ostream& os, const Command& C);
};

#endif  //FTQUANT_SYNTAXPARSER_HPP
