#ifndef FTQUANT_SYNTAXPARSER_HPP
#define FTQUANT_SYNTAXPARSER_HPP

#include <ftqlib.hpp>

#include <fstream>
#include <iostream>
#include <map>
#include <string>
#include <sstream>
#include <string_view>

namespace {
/** @var Fields
 * @brief Container for specific keywords
 * Saves the command keywords: names of all the available commands.
 */
std::map<std::string, int> Fields{
    {"INTEREST_RATE", 0}, {"TRAJECTORIES_NUMBER", 2},
    {"STEPS_NUMBER", 4},  {"SIGMA", 6},
    {"FILE", 7},          {"EXP_T", 8},
    {"SPOT_PRICE", 9},    {"FILE_W", 10},
    {"FILE_T", 12},       {"FILE_y", 13},
    // {"ANTITHETIC", 10}
};

/** @var Commands
 * Contains the command keywords: names of all the available commands.
 */
std::map<std::string, int> Commands{
    {"INVALID_COMMAND", -1},      {"BLACK_SCHOLES", 0}, {"LOCVOL", 1},
    {"GENERATE_TRAJECTORIES", 2}, {"EURO_PUT", 3},      {"EURO_CALL", 4}};

/** @var Codes
 * @brief Container for command codes
 * Contains the command codes for command identification in class Command and interaction with the vector RequiredFields.
 */
enum Codes {
  INVALID_COMMAND = -1,
  BLACK_SCHOLES = 0,
  BLACK_SCHOLES_F = 1,
  LOCVOL = 2,
  GENERATE_TRAJECTORIES = 3,
  EURO_PUT = 4,
  EURO_CALL = 5
};

/** @var RequiredFields
 * @brief Container for required fields
 * Contains the fields required for each command to work correctly.
 * The number of string vector in vector of string vectors corresponds to the command code in the enum Codes.
 */
std::vector<std::vector<std::string>> RequiredFields{
    {"INTEREST_RATE", "SIGMA", "EXP_T", "SPOT_PRICE"},
    {"INTEREST_RATE", "FILE"},
    {"FILE_w", "FILE_T", "FILE_y", "SPOT_PRICE"},
    {"TRAJECTORIES_NUMBER", "STEPS_NUMBER", "EXP_T", "SPOT_PRICE"},
    {"STOCK_PRICE", "STRIKE_PRICE"},
    {"STOCK_PRICE", "STRIKE_PRICE"}};
}  // namespace

/** @function is_double
 * @param str
 */
bool is_double(const std::string& str);

/** @class Command
 * @brief Container for the commands
 * Class contains the parsed command. It is possible to create Command from string, execute command, print command and create json from it.
 */
class Command {
  friend class Execution;

 private:
  int _code;
  std::map<std::string, std::string> _key_numbers;

 public:
  Command() : _code(Commands["INVALID_COMMAND"]) {}

  Command(std::string com);

  int code() const;

  std::string to_json() const;

  friend std::ostream& operator<<(std::ostream& os, const Command& C);
};

class Execution {
 private:
  int traj_generated;
  int isBS;
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
