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
  /**
 * Saves the command keywords: names of all the available commands.  
 */
  {"INTEREST_RATE", 0}, {"TRAJECTORIES_NUMBER", 2},
  {"STEPS_NUMBER", 4},  {"SIGMA", 6}};

std::map<std::string, int> Commands{
  /**
 * Saves the command keywords: names of all the available commands.  
 */
    {"INVALID_COMMAND", -1},      {"BLACK_SCHOLES", 0}, {"LOCVOL", 1},
    {"GENERATE_TRAJECTORIES", 2}, {"EURO_PUT", 3},      {"EURO_CALL", 4}};

enum Codes {
  /**
 * Saves the command codes for command identification in class Command and interaction with the vector RequiredFields. 
 */
  INVALID_COMMAND = -1,
  BLACK_SCHOLES = 0,
  LOCVOL = 1,
  GENERATE_TRAJECTORIES = 2,
  EURO_PUT = 3,
  EURO_CALL = 4
};

std::vector<std::vector<std::string>> RequiredFields{
  /**
 * Saves the fields required for each command to work correctly. 
 * The number of string vector in vector of string vectors corresponds to the command code in the enum Codes.  
 */
    {"INTEREST_RATE", "SIGMA"},
    {},
    {"TRAJECTORIES_NUMBER", "STEPS_NUMBER"},
    {"STOCK_PRICE", "STRIKE_PRICE"},
    {"STOCK_PRICE", "STRIKE_PRICE"}};
}  // namespace

bool isDouble(const std::string& str);

/**
 * Class Command saves the parsed command. It is possible to create Command from string, execute command, print command and create json from it.
 */

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
