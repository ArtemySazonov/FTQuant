#ifndef FTQUANT_SYNTAXPARSER_HPP
#define FTQUANT_SYNTAXPARSER_HPP

#include <ftqlib.hpp>

#include <fstream>
#include <iostream>
#include <map>
#include <string>
#include <sstream>
#include <string_view>
#include <vector>

#include <LocalVolatility.hpp>
#include <BlackSholes.hpp>

namespace {
std::map<std::string, int> Fields{
  /**
 * Saves the command keywords: names of all the available commands.  
 */
  {"INTEREST_RATE", 0}, {"TRAJECTORIES_NUMBER", 2},
  {"STEPS_NUMBER", 4},  {"SIGMA", 6},
  {"FILE", 7}, {"EXP_T", 8},
  {"SPOT_PRICE", 9}, {"FILE_W", 10},
  {"FILE_T", 12}, {"FILE_y", 13}, 
  {"STOCK_PRICE", 14}, {"STRIKE_PRICE", 15}
  // {"ANTITHETIC", 10}
  };

std::map<std::string, int> Commands{
  /**
 * Contains the command keywords: names of all the available commands.  
 */
    {"INVALID_COMMAND", -1},      {"BLACK_SCHOLES", 0},  {"BLACK_SCHOLES_F", 1}, {"LOCVOL", 2},
    {"GENERATE_TRAJECTORIES", 3}, {"EURO_PUT", 4},      {"EURO_CALL", 5}};

enum Codes {
  /**
 * Contains the command codes for command identification in class Command and interaction with the vector RequiredFields. 
 */
  INVALID_COMMAND = -1,
  BLACK_SCHOLES = 0,
  BLACK_SCHOLES_F = 1,
  LOCVOL = 2,
  GENERATE_TRAJECTORIES = 3,
  EURO_PUT = 4,
  EURO_CALL = 5
};

std::vector<std::vector<std::string>> RequiredFields{
  /**
 * Contains the fields required for each command to work correctly. 
 * The number of string vector in vector of string vectors corresponds to the command code in the enum Codes.  
 */
    {"INTEREST_RATE", "SIGMA", "EXP_T", "SPOT_PRICE"},
    {"INTEREST_RATE", "FILE"},
    {"FILE_w", "FILE_T", "FILE_y", "SPOT_PRICE"},
    {"TRAJECTORIES_NUMBER", "STEPS_NUMBER", "EXP_T", "SPOT_PRICE"},
    {"STOCK_PRICE", "STRIKE_PRICE"},
    {"STOCK_PRICE", "STRIKE_PRICE"}};
}  // namespace

bool isDouble(const std::string &str);

/**
 * Class Command contains the parsed command. It is possible to create Command from string, execute command, print command and create json from it.
 */

class Command
{
  friend class Execution;

private:
  int _code;
  std::map<std::string, std::string> _key_numbers;

public:
  Command() : _code(Commands["INVALID_COMMAND"]) {}

  explicit Command(std::string com);

  int execute() const;

  int code() const;

  std::string to_json() const;

  friend std::ostream &operator<<(std::ostream &os, const Command &C);
};

class Execution
{
private:
  int traj_generated = 0;
  int isBS = 1;
  std::vector<std::vector<double>> traj;
  std::vector<std::vector<double>> w;
  std::vector<double> T;
  std::vector<double> y;

  BlackScholes BSmodel;
  LocalVolatility LVmodel;

public:
  Execution() {}

  int execute(Command C);
};

#endif  //FTQUANT_SYNTAXPARSER_HPP
